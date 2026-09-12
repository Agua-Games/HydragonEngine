# Análise do `volume_viewport_manipulator.py`

> ## ✅ RESOLVIDO E ARQUIVADO — 2026-09-11
>
> Este documento descreve o estado **anterior** ao problema estar resolvido, e refere código que já
> não existe (`_fallthrough_counter`, o fallback de teclado `carb.input`). Fica como registo da
> análise, não como descrição do sistema atual.
>
> **O desfecho, em três partes:**
>
> 1. **O fall-through foi corrigido primeiro** por uma janela limitada de override de seleção de
>    0,5 s (`SELECTION_OVERRIDE_WINDOW_S`) mais um backstop por frame. Essa correção estava certa e
>    foi verificada em sessão ao vivo.
> 2. **Depois o problema foi eliminado por construção**, ao substituir o overlay por geometria USD
>    real: as bounds são agora `BasisCurves` com `purpose="default"`, que são geometria a sério e por
>    isso o pick nativo acerta nelas diretamente. Já não há pick para sobrepor — a janela de override
>    tornou-se desnecessária em vez de errada.
> 3. **A deteção de overlap passou para o PhysX**, via um collider invisível com `PhysxTriggerAPI`,
>    gerado a partir da mesma medição que desenha a wireframe. O teste analítico em Python
>    (`check_shape_overlap` / `check_overlap`) foi removido.
>
> O overlay está arquivado em
> `source/extensions/hydragon.editor.core/obsolete/volume_viewport_manipulator_obsolete.py`, com a
> explicação em `obsolete/README.md`. O estado atual e as decisões estão em
> `scratch/HANDOFF_volume_gizmo.md`.

## 🔴 O Bug Principal: Seleção "vaza" para a prim atrás do gizmo

O problema está no **mecanismo de "fall-through guard"** que você implementou. Ele tenta corrigir um sintoma, mas a causa raiz é outra — e o guard em si é frágil.

### Por que acontece

1. **`VolumeSelectGesture` não está consumindo o evento.** Quando você clica no `sc.Line` ou `sc.Arc`, o `on_ended()` chama `select_volume()`, que seta a seleção via USD. **Mas o evento de mouse continua propagando** para o viewport nativo do Kit, que faz o próprio raycast e sobrescreve a seleção com a prim que está geometricamente atrás (o `sc.Line` é apenas UI overlay, não geometria Hydra — então o raycast do viewport ignora o gizmo e pega o que está atrás).

2. **O "guard" via `_just_clicked_volume` + `_fallthrough_counter = 2` é um hack.** Ele depende de:
   - A ordem exata dos eventos `SELECTION_CHANGED` do USD.
   - Um contador arbitrário de 2 frames.
   - O `_on_stage_event` re-setar a seleção **depois** que o viewport já a mudou.

   Isso cria uma **corrida de eventos**: em alguns frames o `SELECTION_CHANGED` do viewport chega antes do seu `_update_selection`, e o resultado é o comportamento "pisca-pisca" / seleção trocando.

3. **`VolumeSelectGesture` herda de `sc.ClickGesture` mas não chama `super().on_ended()`** — e o `sc` do Kit **não tem** `ClickGesture` como classe instanciável direta com esse nome estável. O `hasattr(sc, "ClickGesture")` provavelmente é `False`, então a classe cai no `else object` e o `on_ended` nunca é chamado pelo Kit. A seleção só funciona porque você tem `_on_click` como fallback — mas aí você **não tem como marcar o evento como consumido**.

### Correção real

Você precisa **impedir que o viewport nativo processe o clique**. Duas abordagens que funcionam no Kit:

**Opção A — Bloquear o viewport durante o clique (recomendado):**
```python
def on_ended(self):
    # Desabilita o viewport por 1 frame para o raycast nativo não rodar
    import omni.kit.viewport.utility as vp_util
    vp = vp_util.get_active_viewport()
    if vp:
        vp.enabled = False
    HydragonVolumeViewportOverlay.select_volume(self.prim_path)
    # Reabilita no próximo frame
    async def _reenable():
        await omni.kit.app.get_app().next_update_async()
        if vp:
            vp.enabled = True
    asyncio.ensure_future(_reenable())
```

**Opção B — Usar `omni.ui.scene` com `Manipulator` real** (não apenas `Transform` + `Line`), que registra um `Gesture` que consome o evento antes do viewport.

**Opção C — A mais limpa:** Assinar o `Viewport` `mouse_clicked` event stream com `create_subscription_to_pop` **com ordem de prioridade maior** que o seletor nativo, e quando o clique bater num gizmo, dar `return True` (consumido).

### Bug secundário no guard

```python
if sel != [target]:
    usd_context.get_selection().set_selected_prim_paths([target], True)
    return
```
Isso **re-dispara `SELECTION_CHANGED`**, que re-entra no `_on_stage_event`, mas agora `sel == [target]`, então cai no `else` e limpa o guard. Em frames onde o viewport dispara múltiplos eventos, o guard é consumido antes da hora e o fall-through passa.

---

## 🟡 "Over-complicated stuff" (coisas desnecessariamente complexas)

### 1. **Sistema de fallback de cor triplo**
`make_color()` tenta `cl(r,g,b,a)`, depois `cl.color(...)`, depois monta um `uint32` BGRA manualmente. Se `HAS_KIT` é `True`, `cl` **sempre** existe. O fallback inteiro é código morto. Simplifica para:
```python
COLOR_FORCE_DEFAULT = cl(0.18, 0.72, 1.0, 0.85)
```

### 2. **`VolumeSelectGesture` com `try/except` aninhado e `hasattr` defensivo**
```python
if HAS_KIT and sc and hasattr(sc, "ClickGesture"):
    try:
        super().__init__()
    except Exception:
        pass
```
Isso nunca funciona de forma confiável. Ou você usa `sc.ClickGesture` de verdade, ou não herda dele. O `_on_click(self, *args)` como fallback é inútil porque ninguém chama `_on_click` — o Kit chama `on_ended`.

### 3. **Fallback `VolumeGestureManager = object` / `PreventViewportOthers = object`**
Esses aliases não são usados em lugar nenhum. Código morto.

### 4. **`_create_line` com dois caminhos**
```python
if gesture_obj:
    try:
        line = sc.Line(..., intersection_thickness=6.0)
        line.gestures = [gesture_obj]
        return line
    except Exception:
        pass
line = sc.Line(...)
if gesture_obj:
    try: line.gestures = [gesture_obj]
    except Exception: pass
return line
```
O segundo caminho só existe se o primeiro falhar — mas o primeiro só falha se `intersection_thickness` não for aceito, o que **não acontece** em versões atuais. Pode colapsar em uma chamada só.

### 5. **`_register_action_and_hotkey` triplo**
Você registra a mesma hotkey `Shift+V` em **três lugares diferentes**:
- `omni.kit.hotkeys.core` (correto)
- Fallback `carb.input` keyboard subscription (redundante — o hotkey registry já cuida disso)
- Menu item com `hotkey_text="Shift+V"` (só cosmético)

O fallback `carb.input` **duplica o toggle** se ambos dispararem. Remova o fallback ou o registry, não os dois.

### 6. **`_scan_stage_volumes` chama `is_applied` em toda prim da stage**
Em stages grandes (10k+ prims), isso é caro. Já que `HydragonForceVolume.is_applied` provavelmente checa um `apiSchemas` ou atributo, isso é uma travessia O(n) completa **em cada `OPENED` e `ASSETS_LOADED`**. Melhor usar `Usd.PrimRange` com filtro de `Type` ou registrar um `Usd.Notice.ObjectsChanged` para pegar só adições.

### 7. **`_repopulate_scene_graph` faz `destroy()` + recria `Transform`**
```python
entry.destroy()
with self._root_transform:
    entry.transform_node = sc.Transform(visible=self._is_visible)
entry.cached_transform = None
entry.rebuild_lines(prim)
```
Isso destrói e recria **todos** os nós quando o viewport é reatachado. Desnecessário: você pode apenas **reparenter** movendo o `transform_node` existente para o novo `root_transform`. Se o `sc.Transform` não suportar reparent, aí sim recria — mas no caso comum (mesma viewport), o `_ensure_attached` já retorna cedo.

### 8. **`get_target_xform_prim` + `read_dimensions` com dois blocos quase idênticos**
Force e Kill fazem exatamente a mesma coisa com nomes de prim diferentes (`volumes/force_bounds` vs `volumes/kill_bounds`). Parametrize com um dict:
```python
BOUNDS_PRIM = {"Force": "volumes/force_bounds", "Kill": "volumes/kill_bounds"}
```

### 9. **`update_per_frame` lê `shape` toda frame, mas `update_transform` é quem muda**
```python
def update_per_frame(self, prim, is_visible=True):
    shape = self.read_shape(prim)  # <- lê atributo USD toda frame
    if shape != self.cached_shape:
        self.rebuild_lines(prim)
```
Isso é uma leitura de atributo USD por frame por volume. Aceitável para poucos volumes, mas viola o princípio de "zero stage traversals". Melhor assinar `Usd.Notice.ObjectsChanged` no prim e marcar um flag `_needs_rebuild`.

### 10. **Comentários explicando o óbvio / "dançando"**
```python
# CRITICAL: Only set transform_node.transform if matrix ACTUALLY changed!
# Re-assigning transform on static volumes every frame dirties the scene graph,
# causing a 1-frame camera lag / jitter ("dançando") during viewport navigation.
```
O comentário é bom, mas o fato de você precisar dele indica que `sc.Transform.transform` não faz cache interno. Isso é um workaround para uma limitação do Kit — ok manter, mas talvez isolar num helper.

---

## Resumo do que eu faria

1. **Corrigir o bug de seleção** com uma das opções A/B/C acima — o guard atual é uma corrida de eventos que sempre vai falhar em algum caso.
2. **Remover** `VolumeGestureManager = object`, `PreventViewportOthers = object`, `VolumeClickGesture` (alias não usado), o fallback triplo de `make_color`, o fallback `carb.input` de hotkey, e o segundo caminho de `_create_line`.
3. **Parametrizar** `read_dimensions` por tipo de volume.
4. **Trocar `_scan_stage_volumes` por listener de `ObjectsChanged`** se performance importar.
5. **Reparentar** em vez de destruir/recriar em `_repopulate_scene_graph`.

O script é bem escrito e bem comentado, mas carrega muita "defesa contra o Kit" que na prática ou nunca dispara, ou dispara em dobro.