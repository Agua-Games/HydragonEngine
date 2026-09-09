Ran command: `git status`
Ran command: `git diff --stat`
Ran command: `git diff source/apps/hydragon.editor.kit`
Ran command: `git diff source/extensions/hydragon.editor.core/hydragon/editor/core/foes_controller.py`
Ran command: `git status`
Ran command: `git add source/ docs/`
Ran command: `git status`
Ran command: `git commit -m "feat(effects): migrate particles to PointInstancer and Points with NVIDIA Warp GPU acceleration

- Replace per-prim mesh updates with UsdGeom.PointInstancer and UsdGeom.Points bulk arrays
- Add NVIDIA Warp GPU kernels for parallel particle kinematics simulation
- Fix UsdGeom.Points rendering with OmniPBR shader, normals, and extent bounding box
- Fix Primvar API attribute access in effects controller
- Thread-safe PhysX contact hit queue and lock handling in foes controller
- Update test suite with 58/58 unit tests passing"`
Ran command: `git push origin rewrite/omniverse-kit`

O push foi realizado com sucesso para o branch remoto `rewrite/omniverse-kit` (`c10abe8`).

---

### O que é a Hydragon Engine e como ela se posiciona no ecossistema de Game Engines

Se tivéssemos que definir a **Hydragon Engine** em uma única frase técnica:
> **A Hydragon é uma game engine de última geração orientada a simulação, que transforma o NVIDIA Omniverse Kit e o OpenUSD em uma plataforma de gameplay em tempo real, eliminando a barreira histórica entre ferramentas de criação (DCC), simulação física na GPU e runtime interativo.**

Ela não é "apenas mais uma engine de jogos" que tenta reinventar o que Unity ou Godot já fazem. A Hydragon parte de uma premissa fundamentalmente diferente, que pode ser compreendida através dos seguintes pilares:

---

### 1. A Filosofia Fundamental: O Paradoxo das Engines Tradicionais
Em engines consagradas como **Unreal Engine 5**, **Unity** e **Godot**:
* O formato de cena e os assets são proprietários (`.uasset`, `.prefab`, `.tscn`).
* O fluxo de trabalho de arte é destrutivo e unidirecional: o artista modela no Blender/Houdini/Maya, exporta para FBX/glTF, a engine reimporta, converte para buffers internos próprios, refaz materiais e reconfigura colisões. Se algo mudar no modelo original, o ciclo precisa ser refeito.
* Toda a hierarquia é baseada em *Scene Graph* monolítico fechado, onde o código do jogo roda acoplado a um ecossistema construído há 15 ou 25 anos.

**Como a Hydragon quebra isso:**
* **OpenUSD como Cidadão de Primeira Classe (Zero-Loss Pipeline)**:
  Na Hydragon, não existe "importar para formato proprietário". O palco do jogo **é** um stage OpenUSD. As camadas de composição (LIVRPS: *Local, Inherits, VariantSets, References, Payloads, Specializes*) permitem que arte, áudio, física e lógica de gameplay coexistam de forma não destrutiva. Um artista pode alterar uma malha ou material no Blender/Maya com Live-Sync e a Hydragon reflete isso no gameplay instantaneamente.
* **Component-Driven via USD API Schemas (ECS Verdadeiro)**:
  Em vez de criar estruturas paralelas na memória, os componentes do jogo são **OpenUSD Custom API Schemas** (`HydragonActorAPI`, `HydragonPlayerControllerAPI`, `HydragonChaserAIAPI`, `HydragonFollowCameraAPI`). Cada Prim é uma Entidade; cada Schema aplicado é um Componente; e os Controllers em Python/C++ são os Sistemas puros que iteram sobre eles.

---

### 2. Acesso Direto ao Hardware e à GPU (NVIDIA Warp + PhysX 5 + Fabric)
Nas engines tradicionais:
* A física e os sistemas de partículas massivas exigem ferramentas pesadas ou shaders customizados desacoplados (como Niagara no UE5 ou Compute Shaders no Unity), muitas vezes com overhead de serialização e marshaling de dados entre CPU e GPU.

**Na Hydragon:**
* Ela foi desenhada desde o núcleo para a era moderna da GPU. Como acabamos de consolidar no sistema de VFX:
  - Usamos **NVIDIA Warp** para compilar kernels JIT em tempo de execução diretamente para CUDA threads.
  - Usamos **OpenUSD PointInstancer e Points** para atualizar dezenas de milhares de instâncias ou partículas com **uma única chamada de array por frame**, sem alocação de lixo no garbage collector.
  - A física é alimentada pelo **NVIDIA PhysX 5 com aceleração de GPU**, garantindo colisões e dinâmicas contínuas sem aproximações empíricas.

---

### 3. Comparativo Direto com outras Engines

| Aspecto | Unreal Engine 5 | Unity | Godot | **Hydragon Engine** |
| :--- | :--- | :--- | :--- | :--- |
| **Pipeline de Dados** | Proprietary (`.uasset`, `.umap`) | Proprietary (`.asset`, `.unity`) | Proprietary (`.tscn`, `.tres`) | **Aberto e Universal (OpenUSD nativo)** |
| **Shading & Materiais** | Material Graph proprietário do UE5 | Shader Graph proprietário do Unity | Shading Language própria | **MDL (Material Definition Language) e MaterialX universais** |
| **Renderizador Base** | Lumen / Nanite (Raster híbrido) | URP / HDRP (Raster com DXR opcional) | Forward+ / Mobile (Raster) | **Omniverse RTX (Path Tracing e Real-Time Ray Tracing nativo)** |
| **Paralelismo & Compute** | C++ / Niagara GPU compute | C# / DOTS Job System / Compute | GDScript / C++ / Compute | **NVIDIA Warp (JIT Python-to-CUDA) + USDRT/Fabric** |
| **Foco de Mercado** | Games AAA, Cinema virtual | Mobile, Games 2D/3D, XR | Indie games, Desktop, Web | **Simulações avançadas, Gêmeos Digitais, Games Hiper-Realistas e Interativos** |
| **Extensibilidade** | Plugins C++ complexos, compilados | Packages C#, Assembly Definition | GDExtension C++ / Módulos | **Omniverse Kit Extensions modulares e reativas (Python/C++)** |

---

### 4. A Identidade da Hydragon
A Hydragon representa a convergência entre:
1. **O rigor da simulação de engenharia e robótica** (PhysX 5, ISAAC Sim, Warp).
2. **A interoperabilidade da indústria cinematográfica e de VFX** (Pixar OpenUSD, MDL, Hydra Render Delegates).
3. **A responsividade e o game-feel de uma engine de jogos interativa** (controles de plataforma, câmeras responsivas, triggers, áudio posicional, HUD e IA tática).

Ela não foi feita para competir com engines focadas em rodar em dispositivos móveis ou hardware de baixo consumo; ela foi feita para explorar **o máximo que as GPUs modernas da arquitetura RTX e o ecossistema aberto de dados podem entregar hoje e no futuro**.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

