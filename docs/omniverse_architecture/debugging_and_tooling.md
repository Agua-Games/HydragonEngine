# Debugging and tooling: how to get facts out of a running editor

Most of what is documented in this folder was established by *asking the running editor*, not by
reading code. This document records how, so it does not have to be rediscovered.

## 1. A live Python debugger attached to the editor

**[verified]** `repo.bat launch hydragon.editor.kit` starts `omni.kit.debug.python`, which runs a
**debugpy** server. On this branch no extra flag is needed — the extension is enabled by the app.

- `.vscode/launch.json` → **"Python: Remote Attach"**, debugpy, `localhost:3000`,
  `pathMappings` workspace → workspace.
- The mappings are valid only because of the junction described in §4. If they ever break, fix the
  junction, not the mapping.
- Launch chain: `repoman.py launch hydragon.editor.kit` → `kit.exe apps/hydragon.editor.kit`.
- The debugpy server is a **separate process** from `kit.exe` (a `python.exe` child), which matters
  when you are looking for the right process to attach to.

There is also a **"Launch (Developer Mode)"** task that adds `--enable omni.kit.debug.vscode`. Note
that in a `repo.bat launch` command line, Kit's own flags must come **after** `--`:

```
repo.bat launch hydragon.editor.kit -- --enable omni.kit.debug.vscode
```

There is **no** `-d` on `repo launch`. `-d` is a **Kit** flag (`--wait-debugger`), not a repo one.

### What `evaluate` can and cannot do

**[verified]** The debugger's expression evaluator accepts **expressions only**. No statements, no
`import` statements. To call into a module, reach it through an expression:

```python
__import__("omni.physx").get_physx_scene_query_interface().raycast_closest(...)
```

Lambdas also work. This is enough to poke at the live scene, but it is awkward — which is the
argument for the MCP servers in §7.

### Saving a file is not enough

**[verified]** Once a module has been imported, editing the `.py` on disk changes nothing in the
running session. The extension must be **reloaded** (disable → enable
`hydragon.editor.core` via `omni.kit.app.get_app().get_extension_manager()`) or the editor restarted.
A change that "did not work" is very often a change that was never loaded. Always ask before
concluding a fix failed.

## 2. Where the logs are

**[verified]**

- Kit log: `H:/omniverse_files/logs/Kit/hydragon.editor/0.1/kit_<timestamp>.log`
- Kit user config: `H:/omniverse_files/data/Kit/hydragon.editor/0.1/`
- Crash dumps: `crash_<date>.txt` in **that same Kit data folder**

The logs are **not** under `<data>/logs`. Grepping the wrong path wastes a cycle.

## 3. Headless tests must bootstrap `pxr` themselves

**[verified]** The target-deps interpreter (`_build/target-deps/python/python.exe`) has **no `pxr`**.
To test anything that touches USD outside Kit, bootstrap it from the extscache:

```python
sys.path.insert(0, "<extscache>/omni.usd.libs-*")
os.add_dll_directory("<that>/bin")   # the native _tf / _usd DLLs need their siblings
```

### The collapsed-import trap

**[verified]** A module that puts `carb`, `omni.usd` and `pxr` in **one** `try` block collapses all
of those names to `None` outside Kit — **even when `pxr` itself is importable**. So a headless test
touching a Kit-guarded module usually has to patch the module's globals first:

```python
module.HAS_KIT = True
module.Usd = Usd
module.UsdGeom = UsdGeom
module.Gf = Gf
```

And when a test patches a module attribute that may not exist outside Kit, it must record **which
names existed** and **delete** the rest on the way out. Assigning `None` leaves behind a name the
module never had, and the next test then sees a fake binding.

## 4. `_build` is a junction, not a copy

**[verified]** `_build/windows-x86_64/release/exts/hydragon.editor.core/hydragon` is a Windows
**junction** to `source/extensions/hydragon.editor.core/hydragon` (`Attributes: Directory,
ReparsePoint`; `LinkType: Junction`), created by `premake5.lua`'s `prebuild_link`.

Consequences:

- Python edits are live the moment they are saved. **`repo.bat build` is not needed** for a
  Python-only change.
- `source/` and `_build/.../exts/` **cannot drift**. Any "compare the two copies before trusting the
  fix" advice is wasted work — you would be comparing a file with itself.
- Deleting a file from `source/` removes it from `_build/` immediately.
- A folder created **inside** `hydragon/` ships in the package, because the link is recursive. This
  is why the retired overlay lives in `hydragon.editor.core/obsolete/` at the **extension root**,
  not inside `hydragon/`.

## 5. Writing a probe that runs inside the editor

An in-Kit probe is the fastest way to settle a rendering or cooking question. Rules, all learned by
breaking them:

1. **The first executable statement is `print(..., flush=True)` — before any import.** Without it a
   probe can run silently and you will not know whether it executed at all. Wrap the body in
   `try` / `except` and `traceback.print_exc()`.
2. **Never author into the user's open stage.** Build fixtures in a private
   `Usd.Stage.CreateInMemory()`. A probe that authored into the live stage took the artist's scene
   black (lights gone) and produced no console output — an unrecoverable-looking failure caused by a
   script that was only trying to help.
3. **Gate Kit-only sections behind `HAS_KIT`.** Without it the classes short-circuit to defaults and
   the output is a wall of false failures.
4. Prefer a headless regression test over a manual probe once the question is understood — the probe
   answers "what is true", the test keeps it true.

## 6. Shell traps

**[verified]** Do **not** pipe a long `repo.bat` command through `Select-Object -Last N`. It buffers
the entire stream and emits nothing until the process exits, so a normal run looks hung. Let it
stream, or redirect to a file.

Multi-line PowerShell here-strings in this environment frequently return truncated output. When the
result is not what you need, put the script in a file and run the file.

## 7. MCP servers for Omniverse

Researched, **not installed**. Recorded so the options are known.

**[verified]** `NVIDIA-Omniverse/kit-usd-agents` ships four MCP servers: **USD Code**, **Kit**,
**OmniUI**, **Isaac Sim**. They are **documentation / RAG** servers — they search 400+ extensions,
API docs and code examples. They answer *"how do I use X"*, not *"do X in my running scene"*.

**[verified]** A community server (`nvidia-omniverse-isaacsim-mcp-server`) talks **WebSocket** to Kit
extensions and **does** manipulate the live scene (create rooms, add lights). That is the
"full editor control" class of tool; stability depends on its author.

Practical notes:

- The Kit MCP server needs an `NVIDIA_API_KEY` (from build.nvidia.com) for embeddings. Treat it as a
  **secret**: the user sets it, and it is never routed through a model, a log, or a chat message.
- **A model does not need MCP support — the host is the MCP client.** VS Code + Copilot provides
  MCP; the model behind it is irrelevant to whether MCP works. Statements of the form "model X has
  no MCP support in VS Code" are about model-as-agent, which is not the situation here.

Value for this repository, in order:

1. The **WebSocket scene-control server** — it would replace the expression-only `evaluate`
   workaround in §1.
2. The **Kit MCP** server — reading `extscache` by hand is how several facts in this folder were
   established, and that could be a lookup instead.
3. The **USD Code MCP** server — it would plausibly have documented the `AddXformOp` trap in
   `viewport_drawing_and_gizmos.md` §7 before it bit.
