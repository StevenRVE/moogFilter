cmake_minimum_required(VERSION 3.7)
 project(sve-templatePlugin)

 add_subdirectory(dpf)

 dpf_add_plugin(sve-templatePlugin
   TARGETS clap lv2 vst2 vst3

   FILES_DSP
       source/templatePlugin.cpp
 )

 target_include_directories(sve-templatePlugin
   PUBLIC source)

 target_compile_options(sve-templatePlugin PUBLIC "-Wno-c++11-narrowing")

 add_compile_options(-fdiagnostics-color=always)