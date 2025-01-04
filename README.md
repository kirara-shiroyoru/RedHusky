# RedHusky

This is a VST®3 plugin that act as both instrument and sound analyzer. You can compile one with this project or directly download the RedHusky.vst.zip file.

這是一個能拿來分析聲音，然後將分析結果當成樂器音色使用的VST3插件，你可以自己用這份代碼建一個或直接下載RedHusky.vst.zip

![VST is a trademark of Steinberg Media Technologies GmbH, registered in Europe and other countries.](https://github.com/kirara-shiroyoru/RedHusky/blob/master/resource/VST_COMPATIBLE_LOGO.png)

VST is a trademark of Steinberg Media Technologies GmbH, registered in Europe and other countries.

# Redistribution Policy:

You are free to modify this project and redistribute the modified versions under the terms of the GLP-3.0. However, if you make modifications to this software and redistribute it, you 
**must rename the project**
and
**remove references to the original project name**
including but not limited to name, logo, branding to avoid confusion with the original work.

You
**must not remove VST compatible logo in the modified version unless you remove the sdk used in the project, according to the guideline of the sdk**
https://steinbergmedia.github.io/vst3_dev_portal/pages/VST+3+Licensing/Usage+guidelines.html

**Artwork ("Husky.png" and "main.png") is not part of the project and may only be redistributed with the unmodified version of the plugin or project.**

# Guide Line

## Husky assumption:
All sound on the world can be decomposed into superposition of 16 harmonics with independent ADSRs and volume. Husky sings and learn according to this assumption.

When Husky sings, Husky sings according to the volume (relative, from 0 to 100), attack (ms, from 0 to 10000), decay (ms, from 0 to 10000), sustain (percentage volume of that harmonic, from 0 to 100), and release(ms, from 0 to 10000)

When Husky learns, Husky fit the input to this model, and 
