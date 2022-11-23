<a id="home"></a>
<h1 align="center"> 𝕊𝕖𝕒𝕣𝕔𝕙 𝔼𝕟𝕘𝕚𝕟𝕖 </h1>

[![](/img/intro.png)](https://github.com/shurk4/SearchEngine "Search Engine")

<h5 align="center"> The "Search Engine" is the final project of the course "Developer in C++ from scratch" on the training platform <a href="https://go.skillbox.ru/"> "SkillBox"</a>. </h5>
<h5 align="center">This application searches through text documents</h5>


---

### contents:

* [About](#home) 
* [Technology stack](#tech) 
* [How to use](#use)
  * [Download](#dl)
  * [Windows console compilation](#winconsole)
  * [Windows console compilation](#debconsole)
  * [IDE compilation](#ide)
  * [Using the program](#prgm)

---
<h3 align="center">technology stack</h3>
<a id="tech"></a>

[![](/img/cmake.png)](https://cmake.org/ "CMake")
[![](/img/cpp_stl.png)](https://en.wikipedia.org/wiki/Standard_Template_Library "C++ Standart Template Library")
[![](/img/nloJson.png)](https://json.nlohmann.me/ "Nlohmann JSON")
[![](/img/gtest.png)](https://json.nlohmann.me/ "Google test")
[![](/img/mingw.png)](https://en.wikipedia.org/wiki/MinGW "MinGW")

---
<h3 align="center">How to use</h3>
<a id="use"></a>


<a id="dl"></a>
<details>
  <summary>Download</summary>
        
        Clone the project or download the zip.
  </details>
  
  <a id="winconsole"></a>
<details>
<summary>Windows console compilation</summary>
<ul>
<li>Go to the project folder:</li>

    >cd ..\SearchEngine
    
<li>create a build directory:</li>

    >mkdir build
      
<li>Then go to the build directory and run CMake:</li>

    >cd build
    >cmake ..
    
<li>Then compile the project:</li>

    >cmake --build .

</ul> 
</details>

<a id="debconsole"></a>
<details>
<summary>Debian console compilation</summary>
<ul>

 <details>
 <summary>Preparing for compilation</summary>
 <ul>
     Before installing CMake:
     
    sudo apt update
    sudo apt upgrade
    sudo apt install build-essential checkinstall zlib1g-dev libssl-dev
     
Then follow the official CMake instructions: https://cmake.org/install/
 </ul>
 </details>

<li>Go to the project folder:</li>

    >cd ..\SearchEngine
    
<li>create a build directory:</li>

    >mkdir build
      
<li>Then go to the build directory and run CMake:</li>

    >cd build
    >cmake ..
    
<li>Then compile the project:</li>

    >cmake --build .

</ul>

  </details>
  
  <a id="ide"></a>
<details>
 <summary>IDE compilation</summary>
 This project has been tested on Clion 2021 and Visual Studio 2022 with MinGW and MSVC compilers.
 To compile, just open CMakeList.txt in the root directory of the project in one of the listed IDEs.        

</details>

<a id="prgm"></a>
<details>
 <summary>Using the program</summary>
 
For the "search engine" to work in the directory with the executable file of the program, the following files are required:
 
 config.json - In this file, you must specify the paths to the text files. 
 
     {
     "config": {
        "max_responses": 5,
        "name": "shuriksSearchEngine",
        "version": "0.1"
     },
     "files": [
        "file001.txt",
        "file002.txt",
        "etc..."
     ]
     }
 
 request.json - In this file you need to specify the search queries.
 
     {
     "requests": [
        "some request 1",
        "some request 2",
        "etc..."
     ]
     }
 
 You can also create test files automatically using the program menu.
 
     C - For automatic creation of test configuration and request files
         There are 2 variants of test files
     S - To start engine
     Q - To quit
 
 The file with the search results (result.json) will be located in the directory with the executable file 

</details>

---

[Home](#home)
