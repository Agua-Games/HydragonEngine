/**
 * After fixing, cleaning up, organizing, integrating Object, Node, CommandNode and SceneNode:
 *  - Create a base class for UI managers (UIManagerNode), with only the basic functionality, common to all UI managers.
 *  - Then, Move/duplicate UIManager's current functionality to a new file (UIManagerEditorNode), already in a class and Node encapsulated format, 
 * adapting it to the existing proper methods in Node, like Initialize, Update, etc.
 */