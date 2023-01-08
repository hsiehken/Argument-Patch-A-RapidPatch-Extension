# What is This Project
This is an implementation for the seminar WS2223 in TUM (Technical University of Munich) about Operating Systems and Virtualization by the chair DSE (Distributed Systems & Operating Systems).

In this project, we are showing the possibility to improve the original **RapidPatch** research done by Yi He et. al. reducing the space complexity, risks of making defected patches and increasing the availability of RapidPatch's code correctness verifier "Patch Verifier" by introducing a new patching model `Argumented Patch` (Detail please refer to the [paper](./Extension%20of%20RapidPatch's%20Patching%20Model%3A%20%E2%80%9EArgument%20Patch%E2%80%9C%20and%20Possible%20RapidPatch%20Application%20on%20Closed-Source%20Projects.pdf)).

RapidPatch is a hot-patching framework aiming to solve the enormous patching delay and reduce the risk of deploying patches to running systems. More details can be found over [here](https://www.usenix.org/conference/usenixsecurity22/presentation/he-yi)

# How to Start 
## What you need
 - cmake
 - make
 - GCC

If you are using Ubuntu systems, you can use the command `sudo apt install -y build-essential` to install'em all at once.

You can find 2 folders in the project, which are:
1. Fixed Patch Tiggers
   
   You can use the following command to build the code: 
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

   And then the demo can be started with the command:
   ```
   ./FixedPatchTrigger
   ```

2....TODO

# What We Want to Achieve
1. Fixed Patch Tiggers

    We would like to show that our concept is working with one of RapidPatch's patch triggers model: Fixed Patch Tiggers.

    In the demo, given illustrative vulnerable function is the function `add4` which simply adds 4 integers given and returns the result by adding all of them.

    To prove our concept, our design is followings: when the patch is triggered, the second argument will be intentionally added with a 100 making the result 100 more than the normal result.