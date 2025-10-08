# Unreal_API 5.4.4
## Disclaimer
This repository was created as part of my studies at **AIV (Accademia Italiana Videogiochi)**.  
The copyright of the original material, exercises, and concepts belongs to AIV.  
This repository is intended **only for educational and learning purposes** and does not claim ownership of any copyrighted material from AIV.

***For proper viewing is suggested to manualy compile the project through the .sln file due to errors that can be caused by Unreal Engine.***

# Project Content

## Debug Header
The ***DebugHeader*** namespace defines a set of static helper functions used throughout the plugin to replace Unreal’s default debugging methods.
Instead of relying on scattered UE_LOG or GEngine->AddOnScreenDebugMessage calls, all messages, warnings, and notifications are routed through these functions for consistency and clarity:

-**ScreenPrint**: Displays color-coded debug messages directly on the screen;

-**LogPrint**: Sends formatted log messages to the Unreal console under a unified log category;

-**ShowDialog**: Opens warning or confirmation dialogs in the editor;

-**ShowNotifyInfo**: Displays short-lived pop-up notifications using the Slate system;

## Quick Asset Action

Defines a series of editor-only functions that automate common asset-related tasks within Unreal Engine:
>Each function leverages the DebugHeader system for messages, logs, and notifications;

### Test Action:

A simple function that verifies the plugin’s functionality. It displays a confirmation message on Screen and Log.

### Duplicate:

Duplicates all currently selected assets in the editor a specified number of times:
-Each new asset receives an incremental suffix (e.g., _Mk1, _Mk2, etc.), and the operation automatically saves and logs its results;
-Invalid input prompts a dialog;

### FixPrefix:

-Enforces proper naming conventions by ensuring selected assets have the correct class-based prefix (ex: SM_, T_, M_);
-Assets missing or incorrectly prefixed are renamed automatically, with special handling for material instances;

### BatchRename:

-Performs bulk renaming operations by adding custom prefixes or suffixes to selected assets;
-If both fields are left empty, it defaults to the prefix-fixing behavior;
-The function supports Epic Games’ naming conventions;

### RemoveUnused:

-Scans the selected assets and deletes those that have no references elsewhere in the project;
-If no unused assets are found, the function displays a dialog notifying the user;

### FixRedirectors:

Finds and resolves redirector assets within the /Game directory to ensure clean asset references;

## SlateWidgets

### MyCustomWidget

A fully interactive Slate widget demonstrating multiple UI elements and input handling:
-Text blocks for dynamic display;
-Buttons with bound click events;
-Check boxes using lambda expressions for state changes;
-Editable text boxes for live text updates and commits;
-Combo boxes with dynamically generated entries and selection handling;
-Sliders for numeric input;
-List views showing custom rows and data structures;

### SceneAssetViewer

A widget that provides a live view of all assets in the editor’s current world:
-Automatically gathers blueprints, static meshes, and materials;
-Deduplicates assets to avoid repeated entries;
-Displays asset type and path in a list view;
-Includes buttons that navigate directly to assets in the Content Browser;

### QuickActions:

This module is the main part of the plugin that connects everything together inside the Unreal Editor. It’s responsible for making the tools show up in the editor and for keeping the Slate widgets and asset actions organized:
-Adds new options to the Content Browser menu, like deleting unused assets or fixing redirectors;
-Opens custom editor tabs that contain the Slate widgets(see *MyCustomWidget* and *SceneAssetViewer*);
-Handles the overall setup and cleanup of the plugin when the editor starts or shuts down;
