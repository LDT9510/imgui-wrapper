# imgui_c

Wrapped ImGui version: `1.92.2` (bindings `0.16`)

This little helper wraps [Dear ImGui](https://github.com/ocornut/imgui) and 
[Dear Bindings](https://github.com/dearimgui/dear_bindings) in a ready to use C library, built
with CMake. This is done manually, a rough overview of the process:

- Download the release version of the ImGui source code.
- Download the release version of the Dear Bindings source code, pick the correct one for the version
  of Dear Imgui.
- All original sources and headers go into `src`, only Dear Bindings headers are exposed in `include/imgui_c`.
- The backends are ported as necessary, not using the bindings generation feature, original backends
  go into `src/original_backend`, and a public wrapper is created at `include/imgui_c/backend`, with
  a corresponding private implementation at `src/backend_wrapper`. (_see note below about backends_)
- Optionally port the corresponding backend examples, can be used as tests. Some examples
  require dependencies, that are included as submodules.
- I included "binary_to_compressed_c.cpp" at `tools/` because is useful.
- Update the versions at the top of this README.

**IMPORTANT**: Since this is intended to be a simple convenience wrapper, user code should be 
responsible for linking any required libraries for the backends (for example, SDL), for reference
see the examples provided in the `example` folder.

## ImGui configuration

Imgui can be configured using "imconfig.h", it is empty in this repo, the user should define 
`#define IMGUI_USER_CONFIG` in their project build system pointing to a file to be used as 
configuration, refer to `src/imconfig.h` for options.
