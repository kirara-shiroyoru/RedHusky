# Contact Me

whattoeat0307@gmail.com

# About this project (Red Husky)

This is a VST®3 plugin that act as both instrument and sound analyzer. You can compile one with this project or directly download the RedHusky.vst.zip file. For more detail information check [user guide line](#user-guide-line). There exists a [UI bug](#bug) that I cannot fix but would not prevent you from using this plugin, and I would be glad to fix it if anyone would tell me how to. I would also be happy to recieve any [.redhusky file](#redhusky) as a result of analyzing sound with this Red Husky plugin.

這是一個能拿來分析聲音，然後將分析結果當成樂器音色使用的VST3插件，你可以自己用這份代碼建一個或直接下載RedHusky.vst.zip。然後這個插件有一個我修不好的[UI bug](#bug)，但不至於影響使用，如果有人能告訴我該怎麼修理這個bug我會很感激的。然後如果有人分析完自己有的sample之後得到什麼有趣的[.redhusky檔案](#redhusky)願意分享給我我也會很高興。

![VST is a trademark of Steinberg Media Technologies GmbH, registered in Europe and other countries.](https://github.com/kirara-shiroyoru/RedHusky/blob/master/resource/VST_COMPATIBLE_LOGO.png)

VST is a trademark of Steinberg Media Technologies GmbH, registered in Europe and other countries.

# Redistribution Policy

You are free to modify this project and redistribute the modified versions under the terms of the GLP-3.0. However, if you make modifications to this software and redistribute it, you 
**must rename the project**
and
**remove references to the original project name**
including but not limited to name, logo, branding to avoid confusion with the original work. When redistributing modified or original project, you are required to include a copy of this README.md file or provide a link to the original project's repository where this README.md can be found. This is to ensure proper attribution and to provide necessary information about the project.

You
**must not remove VST compatible logo in the modified version unless you remove the sdk and any code generated by sdk that is used in this project, according to [the guideline of the sdk](https://steinbergmedia.github.io/vst3_dev_portal/pages/VST+3+Licensing/Usage+guidelines.html)**

**Artwork ("Husky.png" and "main.png") is not part of the project and may only be redistributed with either following condition met:**
1. in a **original** plugin or project.
2. otherwise permitted by me, [you can contact me with the email above](#contact-me).

# User Guide Line

## Husky's Assumption

All timbre of sound on the world can be decomposed into superposition of 16 harmonics with independent ADSRs and volume. Husky sings and learns according to this assumption. There is no guarentee that the real world sound is anything close to this assumption, and so is the result of singing and learning.

## What Husky Can Do

You can simply click Husky in the Red Husky plugin to tell Husky which task to do.

### Singing

When Husky sings, Husky sings according to the [timbre information](#timbre-information), [frequncy information](#frequency-information) and midi.

### Learning

When Husky learns, Husky fits the input to Husky's Assumption, and cap the loudest harmonic at 100, according to sound input(only according to the left channel), [main frequency](#main-frequency) and midi. Make sure you give Husky the midi information otherwise Husky simply ignore all the sound inputs.

## Parameters

### Timbre Information:

Automating the timbre when playing a midi note **DOES** affect the up coming sound produced. 

#### volume

Relative, from 0 to 100. Each harmonic has its own volume.

#### attack, deacay, release

In miliseconds, from 0 to 10000. Each harmonic has its own attack, decay and release.

#### sustain

Percentage of volume parameter, from 1 to 100. Each harmonic has its own sustain.

### Frequency Information:

#### main frequency

In Hz, from 1 to 22000, representing the A4 note frequency. Automating the timbre when playing a midi note **DOES** affect the up coming sound produced. 

#### tuning

You can tune the each key any way you like, with value from 1 to 2. All keys are defaulted at 12 equal temperament. Automating the timbre when playing a midi note **DOES NOT** affect the up coming sound produced, since frequency information in a single note is relative frequency based rather than pitch based.

## Presets

You can save and load a preset in form of .redhusky file. 

### Save

Press Save button to save the current state of Red Husky plugin to a preset file. The dialog that pops up when loading a file is based on [tinyfiledialogs](https://github.com/native-toolkit/libtinyfiledialogs), including tinyfiledialogs.h and tinyfiledialogs.c, and is not part of my work.
 **This button DOES NOT save the plugin state in the host**. 

### Load

Press Load button to open and load a existing .redhuskyfile. The dialog that pops up when loading a file is based on [tinyfiledialogs](https://github.com/native-toolkit/libtinyfiledialogs), including tinyfiledialogs.h and tinyfiledialogs.c, and is not part of my work.

### .redhusky

File format that is used in this Red Husky plugin: should only be in length of exactly 376 bytes, saving 94 floats in the file. **Any file with the wrong file extension or length cannot be read by the Red Husky plugin.**

## Bug

### Existing

1. After loading a .redhusky file, you have to reopen the UI to see the value change of all parameters.

### Already Worked Around:

1. Processor is informed several times when load or save tag is called.
2. load and save tag is touched when opening up the UI interface. This bug still exists if you are using a debug mode Red Husky plugin and using the sdk that I had used to to create the UI, resulting in a single pop up dialog. 
