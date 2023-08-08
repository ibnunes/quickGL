<div id="top"></div>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
<div align="center">

<!-- [![Contributors][contributors-shield]][contributors-url] -->
<!-- [![Forks][forks-shield]][forks-url] -->
<!-- [![Stargazers][stars-shield]][stars-url] -->
[![Issues][issues-shield]][issues-url]
[![GPL3 License][license-shield]][license-url]
<!-- [![LinkedIn][linkedin-shield]][linkedin-url] -->

</div>
<br />
<div align="center">
    <a href="https://github.com/ibnunes/quickGL">
        <img src="images/quickGL.png" alt="quickGL">
    </a>
    <h3 align="center">quickGL</h3>
    <p align="center">
        A quick and easy to use OpenGL wrapper
        <br />
        <br />
        <a href="https://github.com/ibnunes/quickGL/issues">Report Bug</a>
        ·
        <a href="https://github.com/ibnunes/quickGL/issues">Request Feature</a>
    </p>
</div>

<!-- ABOUT THE PROJECT -->
## About the project

**quickGL** aims to provide a thin wrapper for OpenGL in order to automate common tasks and provide some useful features, such as:

- Automatic GLFW and GLAD initialization;
- Shader manager:
  - Name each program for easy access;
  - Automatic shader compilation and program linkage;
  - Detailed report for compilation and linkage errors;
- Automatic basic camera handling;
- Callback management:
  - Default callbacks are provided;
  - Any callback can be overriden;
  - Optionally use the default callback within the new one.

<!--
![Image]()
-->

<p align="right">(<a href="#top">back to top</a>)</p>


### Requirements

* [C++ 20](https://cplusplus.com/)
* [GLFW 3.3.8](https://www.glfw.org/)
* [GLM 0.9.9.8](https://github.com/g-truc/glm)
* [GLAD](https://glad.dav1d.de/)

<p align="right">(<a href="#top">back to top</a>)</p>


## Changelog

| Version | Description |
| --- | --- |
| 0.3.1 | Adds initial support for definition of methods for input processing and pre-processing tasks. |
| 0.3.0 | Adds initial support for mouse and camera control. |
| 0.2.0 | Adds shader manager (with support for vertex, fragment, geometry and compute shaders). |
| 0.1.0 | Initial version for automatic GLFW and GLAD initialization. |

<!-- USAGE EXAMPLES -->
## Usage

### Basic structure

```cpp
#define QGL_GLAD_LOCAL
#include "quickgl.hpp"

int main(int argc, char const *argv[]) {
    QGlScene scene(argv[0]);
    scene.initialize(1000, 500, "quickGL smallest example");
    if (!scene.launchSuccessful())
        std::cerr << "Launch was not successful." << std::endl;
    else
        scene.run();
    return 0;
}
```

If you use GLAD, you must define one of the following **before** including `quickgl.hpp`:

| Macro | Definition |
| --- | --- |
| `QGL_GLAD`       | GLAD is available in the system, or the compiler has a link directive to its location. |
| `QGL_GLAD_LOCAL` | GLAD is available in the relative path `"glad/glad.h"`. |

<p align="right">(<a href="#top">back to top</a>)</p>


### Define tasks for each frame

quickGL runs a loop which performs the following functions by order:

| # | Method | Description | Has default? |
| --- | --- | --- | --- |
| 1 | `preProcessInput()` | Tasks to be performed before processing input | No |
| 2 | `processInput()`    | Input processing (mouse events, keyboard...) | **Yes** |
| 3 | `refresh()`         | Frame renderization tasks | No |

By default, `processInput()` exits the application when the Escape key is pressed and moves the camera with WASD keys.

```cpp
void myRefresh(QGlScene& cls) {
    // some graphical code...

    /* It is NOT necessary to swap buffers nor poll events.
     * This is automatically done by quickGL. */
}

scene.refresh = myRefresh;
```

<p align="right">(<a href="#top">back to top</a>)</p>


### Add a personalized callback function

Callback functions **must** be declared inside namespace `qgl::callback`. Access to your `QGlScene` instance is done via the method `getInstance()`: quickGL automatically attaches the instance before rendering the next frame.

| Callback | Set method |
| --- | --- |
| Frame buffer size  | `setFrameBufferSizeCallback()` |
| Mouse button click | `setMouseButtonCallback()` |
| Cursor position    | `setCursorPositionCallback()` |
| Mouse scroll       | `setScrollCallback()` |

The following example applies to mouse button clicks:

```cpp
namespace qgl::callback {
    void myMouseClick(GLFWwindow *window, int button, int action, int mods) {
        getInstance()->finalize();
    }
}

// Whener you need it, add the line:
scene.setMouseButtonCallback(qgl::callback::myMouseClick);
```

<p align="right">(<a href="#top">back to top</a>)</p>


### Add and use shaders

A set of shaders constitues a **program**. You can add a new program by name and access it with the method `withProgram("foo")`: if the program named `foo` does not exist, it is automatically created.

```cpp
bool result =       // Stores the result of program building
    scene.withProgram("triangle")   // Name for the program with the following shaders
    .withShaders(
        "shaders/triangle.vert",    // Path to vertex shader
        "shaders/triangle.frag"     // Path to fragment shader
    )
    .build();                       // Compile shaders and link program

if (!result) {
    // Print the report generated by the shader manager
    std::cerr << scene.withProgram("triangle").getReport() << std::endl;
}

// You can also check at anytime if the build was successful like so:
if (!scene.withProgram("triangle").wasSuccessful()) {
    // potatoes...
}
```

The type of shaders is automatically determined by the number os parameters given to `withShaders()`:

- 1 &mdash; compute shader (because it requires a whole program for itself);
  - `withShader("computeShader")`
- 2 &mdash; vertex and fragment shaders, respectively;
  - `withShader("vertexShader", "fragmentShader")`
- 3 &mdash; same as 2, with the optional geometry shader as 3rd argument.
  - `withShader("vertexShader", "fragmentShader", "geometryShader")`

You can change an uniform variable within the shaders as well:

```cpp
scene.withProgram("triangle").setInt("bar", 42);
```

Whenever you need to proceed your application with this program, call `use()`:

```cpp
scene.withProgram("triangle").use();
```

<p align="right">(<a href="#top">back to top</a>)</p>


### Access camera and mouse data and methods

| Method | Description |
| --- | --- |
| Mouse |
| `withMouseData()` | Retrieves the mouse data handler |
| `getMouseData()`  | ⚠️ **Deprecated!** Same as `withMouseData()` |
| `setMouseData()`  | Sets cursor position and if it shall be considered as first position |
| Camera |
| `withCamera()`    | Retrieves the camera manager handler |

```cpp
scene.setMouseData(x, y, false);
scene.withMouseData().lastX *= .75f;

scene.withCamera().processMouseMovement(x, y);
```

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- RELATED PROJECTS -->
## Related projects

The following projects use **quickGL** as a basis and inspired its initial features:

* [Bohr](https://github.com/ibnunes/Bohr) &mdash; A very small PDB molecular visualizer;
* [MarchGL](https://github.com/AshKetshup/MarchGL) &mdash; An isosurface renderer using marching cubes on modern OpenGL.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the GNU General Public License v3.0. See [`LICENSE.md`][license-url] for more information.

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- CONTACT
## Contact

Igor Nunes &mdash; (...)

Project Link: [https://github.com/ibnunes/quickGL](https://github.com/ibnunes/quickGL)

<p align="right">(<a href="#top">back to top</a>)</p>
-->


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [Joey de Vries](http://joeydevries.com), for his amazing website [Learn OpenGL](https://learnopengl.com/).

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
<!-- [contributors-shield]: -->
<!-- [contributors-url]: -->
<!-- [forks-shield]: -->
<!-- [forks-url]: -->
<!-- [stars-shield]: -->
<!-- [stars-url]: -->
[issues-shield]: https://img.shields.io/github/issues/ibnunes/quickGL.svg?style=for-the-badge
[issues-url]: https://github.com/ibnunes/quickGL/issues
[license-shield]: https://img.shields.io/github/license/ibnunes/quickGL.svg?style=for-the-badge
[license-url]: https://github.com/ibnunes/quickGL/blob/master/LICENSE.md
<!-- [linkedin-shield]: -->
<!-- [linkedin-url]: -->
