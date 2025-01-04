# RedHusky

This is a VST®3 plugin that act as both instrument and sound analyzer. You can compile one with this project or directly download the RedHusky.vst.zip file. There is a [UI bug](#bug) that I cannot fix but would not affect using the plugin. I would be glad if anyone would tell me how to fix it. I would also be happy to recieve any .redhusky file as a result of analyzing sound with this plugin.

這是一個能拿來分析聲音，然後將分析結果當成樂器音色使用的VST3插件，你可以自己用這份代碼建一個或直接下載RedHusky.vst.zip。然後這個插件有[UI bug](#bug)，但不至於影響使用。如果有人能告訴我該怎麼修理這個bug我會很感激的。然後如果有人分析完自己有的sample之後得到什麼有趣的.redhusky檔案願意分享給我我也會很高興。

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

# User Guide Line

## Husky's Assumption:

All sound on the world can be decomposed into superposition of 16 harmonics with independent ADSRs and volume. Husky sings and learn according to this assumption. 

There is no guarentee that the real world sound is anything close to the model, and so is the result of singing and learning.

## What Husky Can Do:

You can simply click Husky in the plugin to tell Husky which task to do.

### Singing:

When Husky sings, Husky sings according to the volume (relative, from 0 to 100), attack (ms, from 0 to 10000), decay (ms, from 0 to 10000), sustain (percentage volume of that harmonic, from 0 to 100), and release(ms, from 0 to 10000)

### Learning:

When Husky learns, Husky fits the input to this model, and cap the loudest harmonic at 100. Note that both sound and midi is required for the plugin to analyze the sound. The plugin state has to be saved to see the learning result, and the Husky's task should automatically change back to Singing.

## Bug
