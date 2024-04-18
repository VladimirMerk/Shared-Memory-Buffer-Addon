{
  "conditions": [
      [
        "OS=='win'", {
          "variables": {
            "include_dirs": [
              "<!(echo %BOOST_ROOT%)"
            ]
          }
        }
      ],
    ],
  "targets": [
    {
      "target_name": "shared-memory-buffer",
      "sources": [ "cppsrc/main.cpp" ],
      "include_dirs": [ 
        "<!(echo %BOOST_ROOT%)",
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "libraries": [ ],
      "cflags": [ "-std=c++11" ],
      "cflags_cc": [ "-std=c++11" ],
      'dependencies': [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'defines': [
        'NAPI_DISABLE_CPP_EXCEPTIONS'
      ],
      "msvs_settings": {
        "VCLinkerTool": {
          "AdditionalLibraryDirectories": [
            "<!(echo %BOOST_ROOT%/stage/lib)"
          ]
        }
      }
    }
  ]
}
