# 📋 Documento de Design: HydragonCharacter Standard Asset

> **Nota:** Este documento reflete o estado atual do design do HydragonCharacter. Valores específicos de propriedades (como raio da cápsula de colisão, velocidades, etc.) são aproximações iniciais e podem ser ajustados durante o desenvolvimento e testes de gameplay.

## 1. Visão Geral

O `HydragonCharacter` é o asset padrão para personagens jogáveis e NPCs na engine Hydragon. Ele encapsula toda a funcionalidade de um personagem de jogo em tempo real, incluindo malhas esqueletais, animações, física, áudio, inteligência artificial e controle de entrada, seguindo as melhores práticas da indústria para USD e Omniverse Kit.

### 1.1. Filosofia de Design

- **Modularidade**: Cada componente (mesh, esqueleto, física, áudio) é um arquivo USD independente, permitindo substituição e reutilização.
- **Composição USD**: Utiliza `references` e `payloads` para composição lazy loading e streaming.
- **Extensibilidade**: Baseado em schemas USD customizados (`HydragonCharacter`) que podem ser estendidos.
- **Portabilidade**: Assets auto-encapsulados que funcionam em qualquer aplicação USD.
- **Pipeline-Friendly**: Estrutura organizada para facilitar a substituição de componentes por artistas.

## 2. Estrutura de Pastas e Arquivos

### 2.1. Localização na Extensão Hydragon

```
/alpha/source/extensions/hydragon.editor.core/
    data/
        assets/
            characters/
                hydragon_character/                    # Pasta raiz do asset
                    hydragon_character.usda            # Asset principal (assembly)
                    animations/                        # Animações do personagem
                        kowra_idle.usda
                        kowra_walk.usda
                        kowra_run.usda
                        kowra_jump.usda
                    lookdev/                           # Materiais e look development
                        kowra_body_mat.usda
                        kowra_head_mat.usda
                    skinned_meshes/                    # Malhas esqueletais
                        kowra_skinned_meshes.usda      # Mesh + skeleton da Kowra
                    textures/                          # Texturas
                        kowra_baseColor.png
                        kowra_normal.png
                        kowra_roughness.png
                        kowra_metallic.png
```

### 2.2. Convenção de Nomenclatura

| Elemento | Convenção | Exemplo | Justificativa |
|----------|-----------|---------|---------------|
| **Pastas de Asset** | `snake_case` com prefixo do estúdio | `hydragon_character/` | Namespace para evitar conflitos |
| **Arquivos USD** | `snake_case` com prefixo do estúdio | `hydragon_character.usda` | Consistência com pastas e validação SimReady |
| **Prims USD** | `snake_case` | `hydragon_character_root` | Consistência com nomes de arquivos |
| **Root Joint** | `root` | `root` | Convenção da indústria (Pixar, Maya) |
| **Prim Skeleton** | `skeleton` | `skeleton` | Nome descritivo para não repetir "root" |
| **Scope de Materiais** | `Looks` | `Looks` | UI do Omniverse Kit espera maiúscula (Create > Material) |

## 3. Estrutura Hierárquica do Asset

### 3.1. Arquivo Principal: `hydragon_character.usda`

```usda
#usda 1.0
(
    defaultPrim = "hydragon_character_root"
    kind = "assembly"
)

def Xform "hydragon_character_root"
{
    # ============================================================
    # COMPONENTE: SkelRoot (Personagem Base)
    # ============================================================
    # O SkelRoot é um componente substituível. O usuário pode trocá-lo
    # por outro personagem sem afetar o restante do asset.
    # O tipo da prim é explicitamente SkelRoot para indicar o que
    # deve ser plugado aqui para o sistema funcionar.
    # ============================================================
    def SkelRoot "kowra" (
        prepend payload = @./skinned_meshes/kowra_skinned_meshes.usda@
        kind = "component"
    )
    {
        # O payload carrega o defaultPrim "kowra" do arquivo
        # kowra_skinned_meshes.usda, que é um SkelRoot
    }

    # ============================================================
    # COMPONENTE: Colisões
    # ============================================================
    def Xform "collisions" (
        kind = "group"
    )
    {
        # Cápsula de colisão do Character Controller
        def Capsule "character_capsule" (
            prepend apiSchemas = ["PhysxCharacterControllerAPI"]
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (0, 0.9, 0)
            float3 xformOp:scale = (1, 1, 1)
            uniform token[] xformOpOrder = ["xformOp:translate", "xformOp:scale"]
            float radius = 0.4              # Valor inicial, pode ser ajustado
            float height = 1.8              # Valor inicial, pode ser ajustado
            bool physics:kinematicEnabled = true
        }
    }

    # ============================================================
    # COMPONENTE: Áudio (Persistente)
    # ============================================================
    # Sons contínuos que fazem parte do personagem
    # Sons de ação (impactos, magias) são one-shot via script
    # ============================================================
    def Xform "audio" (
        kind = "group"
    )
    {
        def OmniSound "voice" (
            kind = "subcomponent"
        )
        {
            asset filePath = @./sounds/voice_lines.wav@
            int loopCount = 0               # 0 = toca uma vez (por enquanto)
            token auralMode = "spatial"
            float gain = 0.5
            float innerConeAngle = 60
            float outerConeAngle = 180
        }
        
        def OmniSound "footsteps" (
            kind = "subcomponent"
        )
        {
            asset filePath = @./sounds/footsteps_loop.wav@
            int loopCount = -1              # Loop infinito
            token auralMode = "spatial"
            float gain = 0.3
            bool enableDoppler = true
        }
    }

    # ============================================================
    # COMPONENTE: Input Mappings
    # ============================================================
    def Xform "input" (
        kind = "group"
    )
    {
        # Custom schema para mapeamento de inputs
        def "input_mapping" (
            prepend apiSchemas = ["HydragonInputAPI"]
            kind = "subcomponent"
        )
        {
            # Propriedades expostas na UI via schema
            token move_forward = "W"
            token move_backward = "S"
            token move_left = "A"
            token move_right = "D"
            token jump = "Space"
            token sprint = "Shift"
            token attack = "MouseLeft"
            token interact = "E"
        }
    }

    # ============================================================
    # COMPONENTE: Equipamento (Itens Equipáveis)
    # ============================================================
    def Xform "equipment" (
        kind = "group"
    )
    {
        # Points de ancoragem para props/armas
        # Exporte seus props com os pivôs já orientados e posicionados
        # para que possam ser diretamente parentados ou referenciados aqui
        # ============================================================
        def Xform "hand_prop_l" (
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (0.2, -0.1, 0.3)
            float3 xformOp:rotateXYZ = (0, 90, 0)
            uniform token[] xformOpOrder = ["xformOp:translate", "xformOp:rotateXYZ"]
        }

        def Xform "hand_prop_r" (
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (-0.2, -0.1, 0.3)
            float3 xformOp:rotateXYZ = (0, -90, 0)
            uniform token[] xformOpOrder = ["xformOp:translate", "xformOp:rotateXYZ"]
        }

        def Xform "back_prop" (
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (0, 1.2, -0.2)
            uniform token[] xformOpOrder = ["xformOp:translate"]
        }
    }

    # ============================================================
    # COMPONENTE: Câmeras
    # ============================================================
    # Cada câmera tem um Xform de pivô para permitir ajustes
    # de orientação independentes da câmera em si
    # ============================================================
    def Xform "cameras" (
        kind = "group"
    )
    {
        def Xform "follow_cam_pivot" (
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (0, 2, -5)
            float3 xformOp:rotateXYZ = (10, 0, 0)
            uniform token[] xformOpOrder = ["xformOp:translate", "xformOp:rotateXYZ"]

            def Camera "follow_cam"
            {
                float focalLength = 35
                float horizontalAperture = 36
            }
        }

        def Xform "first_person_cam_pivot" (
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (0, 1.7, 0.2)
            uniform token[] xformOpOrder = ["xformOp:translate"]

            def Camera "first_person_cam"
            {
                float focalLength = 50
                float horizontalAperture = 36
            }
        }
    }

    # ============================================================
    # COMPONENTE: IA e Navegação
    # ============================================================
    def Xform "ai" (
        kind = "group"
    )
    {
        # Árvore de Comportamento (Behavior Tree)
        def "behavior_tree" (
            prepend references = @./behavior_trees/idle_behavior.usd@
            kind = "subcomponent"
        )
        {
        }

        # Volume de NavMesh (se necessário)
        def NavMeshIncludeVolume "nav_volume" (
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (0, 0, 0)
            float3 xformOp:scale = (10, 10, 10)
            uniform token[] xformOpOrder = ["xformOp:translate", "xformOp:scale"]
        }
    }

    # ============================================================
    # COMPONENTE: Rig (IK, Effectors, Contacts)
    # ============================================================
    def Xform "rig" (
        kind = "group"
    )
    {
        # Effectors para IK
        def Xform "right_hand_effector" (
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (0, 0, 0)
            uniform token[] xformOpOrder = ["xformOp:translate"]
        }

        def Xform "left_hand_effector" (
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (0, 0, 0)
            uniform token[] xformOpOrder = ["xformOp:translate"]
        }

        def Xform "right_foot_effector" (
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (0, 0, 0)
            uniform token[] xformOpOrder = ["xformOp:translate"]
        }

        def Xform "left_foot_effector" (
            kind = "subcomponent"
        )
        {
            float3 xformOp:translate = (0, 0, 0)
            uniform token[] xformOpOrder = ["xformOp:translate"]
        }
    }

    # ============================================================
    # COMPONENTE: Materiais (Biblioteca Central)
    # ============================================================
    # Os materiais são carregados da biblioteca central via payload
    # e bindados diretamente às meshes.
    # O binding é feito dentro de kowra_skinned_meshes.usda usando
    # caminhos relativos para a biblioteca.
    # ============================================================
    def "material_library" (
        prepend payload = @./lookdev/hydragon_material_library.usda@
    )
    {
        # A biblioteca contém:
        #   def Material "kowra_body_mat" { ... }
        #   def Material "kowra_head_mat" { ... }
    }
}
```

### 3.2. Componente: `kowra_skinned_meshes.usda` (SkelRoot)

```usda
#usda 1.0
(
    defaultPrim = "kowra"
    kind = "component"
)

def SkelRoot "kowra"
{
    # ============================================================
    # ESQUELETO
    # ============================================================
    # A estrutura de ossos. O nome "skeleton" é descritivo para
    # a prim Skeleton, enquanto a root joint se chama "root".
    # O array joints lista todos os ossos do topo para as folhas.
    # ============================================================
    def Skeleton "skeleton"
    {
        uniform token[] joints = [
            "root",
            "pelvis",
            "spine_01",
            "spine_02",
            "neck",
            "head",
            "left_shoulder",
            "left_arm",
            "left_forearm",
            "left_hand",
            "right_shoulder",
            "right_arm",
            "right_forearm",
            "right_hand",
            "left_thigh",
            "left_leg",
            "left_foot",
            "right_thigh",
            "right_leg",
            "right_foot"
        ]

        # Bind Transforms (pose T)
        float3[] bindTransforms = [
            (0, 0, 0),
            (0, 0.9, 0),
            (0, 1.2, 0),
            (0, 1.5, 0),
            (0, 1.7, 0),
            (0, 1.85, 0),
            (0.2, 1.6, 0),
            (0.5, 1.6, 0),
            (0.8, 1.6, 0),
            (1.1, 1.6, 0),
            (-0.2, 1.6, 0),
            (-0.5, 1.6, 0),
            (-0.8, 1.6, 0),
            (-1.1, 1.6, 0),
            (0.1, 0.6, 0),
            (0.1, 0.2, 0),
            (0.1, -0.2, 0),
            (-0.1, 0.6, 0),
            (-0.1, 0.2, 0),
            (-0.1, -0.2, 0)
        ]

        # Rest Transforms (pose local de cada joint)
        float3[] restTransforms = [
            (0, 0, 0),
            (0, 0.9, 0),
            (0, 0.3, 0),
            (0, 0.3, 0),
            (0, 0.2, 0),
            (0, 0.15, 0),
            (0.2, 0.1, 0),
            (0.3, 0, 0),
            (0.3, 0, 0),
            (0.3, 0, 0),
            (-0.2, 0.1, 0),
            (-0.3, 0, 0),
            (-0.3, 0, 0),
            (-0.3, 0, 0),
            (0.1, -0.3, 0),
            (0, -0.4, 0),
            (0, -0.4, 0),
            (-0.1, -0.3, 0),
            (0, -0.4, 0),
            (0, -0.4, 0)
        ]
    }

    # ============================================================
    # MALHAS (Visual - Render)
    # ============================================================
    # Container para geometria de renderização (purpose = "render")
    # As malhas ficam diretamente dentro de "render", sem pasta
    # intermediária "meshes" para simplificar a hierarquia.
    # ============================================================
    def Xform "render" (
        purpose = "render"
    )
    {
        def Mesh "body" (
            prepend apiSchemas = ["UsdSkelBindingAPI"]
            kind = "subcomponent"
        )
        {
            # Dados de geometria
            point3f[] points = [...]  # Vértices da malha
            int[] faceVertexIndices = [...]  # Índices de vértices por face
            int[] faceVertexCounts = [...]  # Número de vértices por face
            normal3f[] normals = [...]  # Normais
            float2[] primvars:st = [...]  # UVs

            # Dados de Skinning (influência dos ossos)
            int[] primvars:skel:jointIndices = [...]  # Índices dos joints por vértice
            float[] primvars:skel:jointWeights = [...]  # Pesos por vértice

            # Relação que liga a mesh ao esqueleto
            rel skel:skeleton = </kowra/skeleton>

            # Ligação ao material (biblioteca central)
            # Caminho relativo para a biblioteca carregada no asset pai
            rel material:binding = </hydragon_character_root/material_library/kowra_body_mat>
        }

        def Mesh "head" (
            prepend apiSchemas = ["UsdSkelBindingAPI"]
            kind = "subcomponent"
        )
        {
            # ... dados da cabeça ...
            rel skel:skeleton = </kowra/skeleton>
            rel material:binding = </hydragon_character_root/material_library/kowra_head_mat>
        }
    }

    # ============================================================
    # MALHAS (Proxy / LOD)
    # ============================================================
    # Geometria de baixa resolução para LODs e física
    # purpose = "proxy" indica que é uma representação simplificada
    # ============================================================
    def Xform "proxy" (
        purpose = "proxy"
    )
    {
        def Mesh "body_proxy" (
            prepend apiSchemas = ["UsdSkelBindingAPI"]
            kind = "subcomponent"
        )
        {
            # ... dados da geometria de baixa resolução ...
            rel skel:skeleton = </kowra/skeleton>
            rel material:binding = </hydragon_character_root/material_library/kowra_body_mat>
        }

        def Mesh "head_proxy" (
            prepend apiSchemas = ["UsdSkelBindingAPI"]
            kind = "subcomponent"
        )
        {
            # ... dados da geometria de baixa resolução ...
            rel skel:skeleton = </kowra/skeleton>
            rel material:binding = </hydragon_character_root/material_library/kowra_head_mat>
        }
    }
}
```

### 3.3. Arquivo de Animação: `kowra_idle.usda`

```usda
#usda 1.0
(
    defaultPrim = "kowra_root"
    kind = "component"
)

# O SkelRoot agrupa o esqueleto e a animação
# para facilitar retargeting e carregamento
def SkelRoot "kowra_root"
{
    # ============================================================
    # ESQUELETO (mesma estrutura que no arquivo de meshes)
    # ============================================================
    # Apenas a definição do esqueleto, sem malhas
    # A ordem dos joints deve ser IDÊNTICA para retargeting
    # ============================================================
    def Skeleton "skeleton"
    {
        uniform token[] joints = [
            "root",
            "pelvis",
            "spine_01",
            "spine_02",
            "neck",
            "head",
            "left_shoulder",
            "left_arm",
            "left_forearm",
            "left_hand",
            "right_shoulder",
            "right_arm",
            "right_forearm",
            "right_hand",
            "left_thigh",
            "left_leg",
            "left_foot",
            "right_thigh",
            "right_leg",
            "right_foot"
        ]

        # Bind Transforms (pose T)
        float3[] bindTransforms = [
            (0, 0, 0),
            (0, 0.9, 0),
            (0, 1.2, 0),
            (0, 1.5, 0),
            (0, 1.7, 0),
            (0, 1.85, 0),
            (0.2, 1.6, 0),
            (0.5, 1.6, 0),
            (0.8, 1.6, 0),
            (1.1, 1.6, 0),
            (-0.2, 1.6, 0),
            (-0.5, 1.6, 0),
            (-0.8, 1.6, 0),
            (-1.1, 1.6, 0),
            (0.1, 0.6, 0),
            (0.1, 0.2, 0),
            (0.1, -0.2, 0),
            (-0.1, 0.6, 0),
            (-0.1, 0.2, 0),
            (-0.1, -0.2, 0)
        ]

        # Rest Transforms (pose local de cada joint)
        float3[] restTransforms = [
            (0, 0, 0),
            (0, 0.9, 0),
            (0, 0.3, 0),
            (0, 0.3, 0),
            (0, 0.2, 0),
            (0, 0.15, 0),
            (0.2, 0.1, 0),
            (0.3, 0, 0),
            (0.3, 0, 0),
            (0.3, 0, 0),
            (-0.2, 0.1, 0),
            (-0.3, 0, 0),
            (-0.3, 0, 0),
            (-0.3, 0, 0),
            (0.1, -0.3, 0),
            (0, -0.4, 0),
            (0, -0.4, 0),
            (-0.1, -0.3, 0),
            (0, -0.4, 0),
            (0, -0.4, 0)
        ]
    }

    # ============================================================
    # ANIMAÇÃO
    # ============================================================
    # A animação está diretamente associada ao esqueleto via
    # skel:animationSource
    # ============================================================
    def SkelAnimation "idle"
    {
        # Array de joints na mesma ordem que o esqueleto
        uniform token[] joints = [
            "root",
            "pelvis",
            "spine_01",
            "spine_02",
            "neck",
            "head",
            "left_shoulder",
            "left_arm",
            "left_forearm",
            "left_hand",
            "right_shoulder",
            "right_arm",
            "right_forearm",
            "right_hand",
            "left_thigh",
            "left_leg",
            "left_foot",
            "right_thigh",
            "right_leg",
            "right_foot"
        ]

        # Dados de animação time-sampled
        quatf[] rotations.timeSamples = {
            0: [
                (1, 0, 0, 0),  # root
                (1, 0, 0, 0),  # pelvis
                (1, 0, 0, 0),  # spine_01
                # ... todos os joints no frame 0
            ],
            30: [
                (0.99, 0.01, 0.01, 0.05),  # root
                (0.99, 0, 0.02, 0.1),      # pelvis
                # ... todos os joints no frame 30
            ],
            60: [
                (0.99, 0.02, 0, 0.05),     # root
                # ... todos os joints no frame 60
            ],
        }

        # Blendshapes (se houver)
        float[] blendShapes.timeSamples = {
            0: [0, 0, 0],
            30: [1, 0.5, 0],
        }
    }
}
```

## 4. Como Usar o HydragonCharacter

### 4.1. Criando um Novo Personagem via UI

1. **Abra o Menu `Create`**: No Omniverse Kit, vá em `Create > Hydragon > Character`.

2. **Selecione um Template**: Escolha entre `Empty Character` (personalizável) ou `Kowra` (personagem de exemplo).

3. **Defina o Nome**: Insira o nome do seu personagem (ex: `my_character`).

4. **O Asset é Criado**: O Kit gera uma cópia do template na sua pasta de projeto, com a estrutura completa.

5. **Personalize**:
   - Substitua as malhas: Substitua o payload `kowra` por seu próprio arquivo USD.
   - Ajuste parâmetros: Na UI de propriedades do schema `HydragonCharacter`.
   - Troque animações: Substitua os arquivos na pasta `animations/` e atualize o Animation Graph.
   - Configure física: Ajuste a cápsula de colisão no Xform `collisions`.

### 4.2. Criando via Script Python (Para Automação)

```python
import omni.usd
import omni.kit.commands
from pxr import Sdf

def create_hydragon_character(name, template_path="/assets/characters/hydragon_character/hydragon_character.usda"):
    """
    Cria um novo HydragonCharacter a partir do template.
    """
    stage = omni.usd.get_context().get_stage()
    character_path = Sdf.Path(f"/World/{name}")
    
    # Cria o asset a partir do template (cópia)
    omni.kit.commands.execute("CreateReference",
        path_to=character_path,
        asset_path=template_path,
        prim_path="/hydragon_character_root"
    )
    
    # Aplica o custom schema
    prim = stage.GetPrimAtPath(character_path)
    if prim:
        # Adiciona o schema HydragonCharacter
        omni.kit.commands.execute("AddSchema",
            prim=prim,
            schema_name="HydragonCharacter"
        )
        
        # Define propriedades padrão
        prim.GetAttribute("speed").Set(100.0)
        prim.GetAttribute("health").Set(100)
    
    return character_path

# Uso
create_hydragon_character("my_hero")
```

### 4.3. Substituindo Componentes (Pipeline Workflow)

**Para substituir a malha e o esqueleto de um personagem:**

1. **Exporte do Maya** seu novo personagem como USD, com a estrutura:
   ```
   def SkelRoot "novo_personagem"
   {
       def Skeleton "skeleton" { ... }
       def Xform "render" { ... }
   }
   ```

2. **Atualize o Payload** no `hydragon_character.usda`:
   ```usda
   def SkelRoot "novo_personagem" (
       prepend payload = @./skinned_meshes/novo_personagem_skinned_meshes.usda@
       kind = "component"
   )
   {
   }
   ```

3. **Ajuste o Binding de Materiais**: Se o novo personagem tiver materiais diferentes, atualize a biblioteca de materiais.

4. **Ajuste as Animações**: Se o novo esqueleto tiver a mesma estrutura de ossos (mesma ordem de joints), as animações existentes funcionarão automaticamente. Caso contrário, use o `UsdSkelAnimMapper` para retargeting.

## 5. Integração com a Engine Hydragon

### 5.1. Schemas Customizados

O `HydragonCharacter` utiliza os seguintes schemas USD customizados:

| Schema | Propósito | Propriedades |
|--------|-----------|--------------|
| **HydragonCharacter** | Asset principal do personagem | `speed`, `health`, `jump_force`, `inventory` |
| **HydragonInputAPI** | Mapeamento de inputs | `move_forward`, `jump`, `attack`, etc. |
| **HydragonCharacterControllerAPI** | Controle de física | `walk_speed`, `sprint_speed`, `gravity` |

### 5.2. Controle em Runtime

```python
# Exemplo de controle do personagem via Python
import omni.physx
import omni.physxcct

# Obtém o character controller
cct = omni.physxcct.get_character_controller("/World/my_hero/collisions/character_capsule")

# Define movimento
cct.set_walk_speed(150.0)
cct.set_sprint_speed(300.0)
cct.set_jump_force(800.0)

# Move o personagem
cct.move((1.0, 0.0, 0.0))  # Move para frente (direção local)

# Aplica pulo
if cct.is_grounded():
    cct.jump()
```

### 5.3. Animation Graph Integration

```python
import omni.anim.graph.core as ag

# Obtém a instância do personagem
char = ag.get_character("/World/my_hero/hydragon_character_root/kowra")

# Controla variáveis do Animation Graph
char.set_variable("Speed", 150.0)      # Velocidade atual
char.set_variable("Direction", (1,0,0)) # Direção do movimento
char.set_variable("IsGrounded", True)   # Se está no chão
char.set_variable("IsSprinting", False) # Se está correndo

# Ativa uma animação específica (ex: soco)
char.set_variable("PunchTrigger", True)
```

## 6. Justificativa das Decisões de Design

### 6.1. Por que `SkelRoot` é uma Prim Tipada no Asset Principal?

- **Clareza Visual**: Ao abrir `hydragon_character.usda`, o usuário vê imediatamente que deve plugar um `SkelRoot` ali.
- **Intenção**: O tipo `SkelRoot` sinaliza que este componente é a base do personagem, e não um Xform genérico.
- **Validação**: Ferramentas de pipeline podem verificar se o tipo correto está sendo usado.

### 6.2. Por que as Meshes Estão Diretamente em `render/` sem `meshes/`?

- **Simplificação**: Reduz a profundidade da hierarquia, facilitando a navegação.
- **Clareza**: Para um artista visual, "render" já indica que é a geometria de renderização.

### 6.3. Por que a Prim Skeleton se Chama `skeleton` e não `root`?

- **Clareza**: A prim `Skeleton` é a estrutura de ossos, enquanto a `root joint` é o primeiro osso.
- **Evita Duplicação**: Ter `Skeleton "root"` e `joint "root"` é confuso. Separar os nomes melhora a legibilidade.

### 6.4. Por que Materiais são Carregados via Payload?

- **Performance**: A biblioteca de materiais pode ser grande. Usar payload permite lazy loading.
- **Modularidade**: A biblioteca de materiais pode ser compartilhada entre vários assets.

### 6.5. Por que `Looks` com Maiúscula?

- **UI do Omniverse Kit**: O comando `Create > Material` espera um Scope chamado "Looks" com maiúscula. Usar minúscula resulta em dois Scopes (o criado manualmente e o criado pela UI).

## 7. Checklist de Validação (SimReady)

Para garantir que o asset esteja em conformidade com as práticas SimReady da NVIDIA:

- [ ] A `defaultPrim` é um `Xform` com `kind = "assembly"`.
- [ ] O `SkelRoot` é um componente (`kind = "component"`) e não a raiz.
- [ ] O Scope de materiais se chama `Looks` com maiúscula.
- [ ] Geometria de renderização tem `purpose = "render"`.
- [ ] Geometria de proxy tem `purpose = "proxy"`.
- [ ] `Xform` para `collisions`, `audio`, `input`, `equipment` com `kind = "group"`.
- [ ] Nomes de arquivo e prims seguem `snake_case` com prefixo `hydragon_`.
- [ ] Caminhos relativos são usados para referências internas.
- [ ] Payloads são usados para componentes pesados (LODs, animações, materiais).
- [ ] `instanceable = true` para geometrias repetidas.

## 8. Referências

- [USD Composition Arc (LIVRPS)](https://graphics.pixar.com/usd/docs/USD-Understanding-the-Composition-Arc.html)
- [UsdSkel Schema](https://graphics.pixar.com/usd/docs/api/_usd_skel__schemas.html)
- [NVIDIA SimReady Assets](https://docs.omniverse.nvidia.com/prod_developer/prod_developer/validation-rules/index.html)
- [Omniverse Kit Extensions](https://docs.omniverse.nvidia.com/kit/docs/kit-manual/latest/guide/extensions_overview.html)
- [OmniGraph Animation](https://docs.omniverse.nvidia.com/anim_graph/docs/intro.html)

--------------------------------------------------------------------------------------------------------------------------------------------------

