# imgui_wrapper

Wrapped ImGui version (non-docking): `1.92.2` (bindings `0.16`)

This little helper wraps (in an opinionated manner) [Dear ImGui](https://github.com/ocornut/imgui) and 
[Dear Bindings](https://github.com/dearimgui/dear_bindings) in a ready to use C/C++ library, built
with CMake. This is done manually, a rough overview of the process:

- Download the release version of the ImGui source code.
- Download the release version of the Dear Bindings source code, pick the correct one for the version
  of Dear Imgui.
- All original sources and non-public headers go into `src`, Dear Bindings headers are exposed at 
  `include/imgui_c`, C++ ones at `include/imgui`.
- The C backends are ported as necessary at `include/imgui_c/backend`, not using the bindings generation 
  feature. Original backends sources go into `src/original_backend`, headers into `include/imgui/backend` 
  and a public C wrapper is created at `include/imgui_c/backend`, with a corresponding private implementation 
  at `src/backend_wrapper` (_see note below about backends_).
- Optionally port to C the corresponding backend examples, can be used as tests. Some examples
  require dependencies, these are included as submodules.
- I included "binary_to_compressed_c.cpp" at `tools/` because it's useful.
- Update the versions at the top of this README.
- Original sources and headers should not be modified in any form.

**IMPORTANT**: Since this is intended to be a simple convenience wrapper, user code should be 
responsible for linking any required libraries for the backends (for example, SDL), for reference
see the examples provided in the `example` folder.

## ImGui configuration

Imgui can be configured using "imconfig.h", it is empty and unused in this repo, the user should 
define `IMGUI_USER_CONFIG` in their project build system pointing to a file to be used as 
configuration, refer to `include/imgui/imconfig.h` for options.
