# 2D Game 

**Description**  
This game is created in OpenGL which is a cross platform for graphic designing.  

Install these libraries (instructions for ubuntu):

```
sudo apt-get update
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

Instructions to test:

```
mkdir build
cd build
cmake ..
make -j
```

__Control__  
Use arrow button for movement.  
Dodge the enemy and reach to gate. If you cross three gates, you won and if enemy eat you, you lost.  
