PTSDInteraction
===============

PTSDInteraction is a utility mod for Satisfactory that simply adds a WidgetInteraction component to every player.
This does nothing by itself and is designed to be a dependency for other mods.

Heavily inspired by (i.e. Copied from...) [Ficsit-Networks](https://github.com/CoderDE/FicsIt-Networks)

Extra
=====

Adds the ability to register widgets that desire keyboard focus:
```
APTSDInteractionSubsystem::EnableTestForKBFocus()

APTSDInteractionSubsystem::RegisterDesiresKBFocusWidget(UWidget* Widget)
APTSDInteractionSubsystem::UnregisterDesiresKBFocusWidget(UWidget* Widget)
```
Unfortunately due to limitations with UMG only clicking somewhere outside of the focused widget will return keyboard focus to the player.

i.e. Pressing enter in a textinput widget is not enough :(