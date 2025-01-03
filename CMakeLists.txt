cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "C:/vst-sdk_3.7.10/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(RedHusky
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "RedHusky VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(RedHusky
    source/version.h
    source/Huskycids.h
    source/Huskyprocessor.h
    source/Huskyprocessor.cpp
    source/Huskycontroller.h
    source/Huskycontroller.cpp
    source/Huskyentry.cpp
    source/HuskyParams.h
    source/HuskySinging.h
    source/HuskySinging.cpp
    source/HuskyLearning.h
    source/HuskyLearning.cpp
    source/HuskyEditor.h
    source/HuskyIBStream.h
    source/HuskyIBStream.cpp

    source/tinyfiledialogs.h
    source/tinyfiledialogs.c
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(RedHusky
        PRIVATE
            resource/Huskyeditor.uidesc
    )
    target_link_libraries(RedHusky
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(RedHusky
        RESOURCES
            "resource/Huskyeditor.uidesc"
            "resource/ADSR.png"
            "resource/keyAdjustAnimation.png"
            "resource/main.png"
            "resource/mainFq.png"
            "resource/Husky.png"
            "resource/Husky.png"
            "resource/VST_COMPATIBLE_LOGO.png"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (RedHusky
    RESOURCES
        resource/C403ADE2BC03549FB4EBD79D04B3B750_snapshot.png
        resource/C403ADE2BC03549FB4EBD79D04B3B750_snapshot_2.0x.png
)

target_link_libraries(RedHusky
    PRIVATE
        sdk
)

smtg_target_configure_version_file(RedHusky)

if(SMTG_MAC)
    smtg_target_set_bundle(RedHusky
        BUNDLE_IDENTIFIER com.shiroyou.redhusky
        COMPANY_NAME "shiroyoru"
    )
    smtg_target_set_debug_executable(RedHusky
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(RedHusky PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RedHusky)

        smtg_target_set_debug_executable(RedHusky
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
