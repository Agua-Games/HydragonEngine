---------------------------------------------------------------------------------------------------------------------------
PIPELINE (CHIMERA, etc)
---------------------------------------------------------------------------------------------------------------------------

- Script python + UI to convert usd assets inside Hydragon to proper specs:

	EXPORTED FROM MAYA:

	- convert materials to Hydragon standard
		- move materials to "looks" scope.
		- mostly Omni Surface.			// will need custom API schema. Refer to Painter's custom schema for tips.
		- reassign textures to proper slots in the materials.
	- all prim names in snake-case.

	EXPORTED FROM BLENDER:

	EXPORTED FROM ZBRUSH:

	EXPORTED FROM PAINTER:

---------------------------------------------------------------------------------------------------------------------------
STAGE EDITING
---------------------------------------------------------------------------------------------------------------------------

- Edit ref/payload content (UI button, right-click command).
	- opens a ref/payload loaded in the current stage for editing its content - another tab? Ideally, in-place. At least,
	opens its usd file.

- Copy prim (its type, attributes) to the clipboard, paste a new prim. Useful to copy content from a ref/payload into the
current stage. I installed the copy prim extensions, must check if it copies from refs/payloads also.

- Snaps
	- snap to vertex
	- snap to face

- isolate selected (view only selected)

- look through selected camera (with shortcut)

- right-click over surface, "snap camera here" (or other appropriate text).

---------------------------------------------------------------------------------------------------------------------------
STAGE CREATION
---------------------------------------------------------------------------------------------------------------------------

- New templates for File > New							// salvos em /data/assets/scripts/new_stage.
	- sublevel. Já vem com organização para: persistent, streaming, Looks, etc.
	- "entity". Já vem com xforms para render, proxy, fx, audio, etc. Scope for Looks.

- Procedural prims, assets
	- procedural array instancer

---------------------------------------------------------------------------------------------------------------------------
LOOKDEV
---------------------------------------------------------------------------------------------------------------------------

- Add more preview light rig presets
	- night with neon (full moon)

---------------------------------------------------------------------------------------------------------------------------
GAMEPLAY
---------------------------------------------------------------------------------------------------------------------------

- intro menu API schema, create command in the UI.
- launchpad (for the marble game, platformers, etc). API schema. Can be used also like a volume/trigger in a path to push player forward via
same impulse with strong force.
- moving platform (moves back and forth). API schema.
- kill volume/trigger.

- play-edit tab/window
- package menu entry + tab/window. To package game (a game is simply a .kit project/app). Get rid of unnecessary bloat:
	- unnecessary services running on update (game/app loop).
	- unnecessary assets.

