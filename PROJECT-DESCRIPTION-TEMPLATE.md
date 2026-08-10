# Dark street

213/2022 - Jovan Mijajlović<br>
This project showcases a night time scene featuring a house, a road, streetlights, and cars.
It implements directional and spot lighting, realistic visualization of light sources through an HDR Bloom effect,
and shadows using Point Shadows.
The scene includes interactive elements - control over streetlight brightness and a car driving event.

## Controls

W, A, S, D -> camera movement (forward, left, backward, right)<br>
SPACE -> move camera up<br>
LEFT SHIFT -> move camera down<br>
MOUSE -> camera rotation<br>
1 -> decrease streetlight brightness<br>
2 -> increase streetlight brightness<br>
ENTER -> event start (taxi and blue car)<br>
R -> reset cars after event<br>
ESC -> exit application

## Features

### Fundamental:

[x] Model with lighting<br>
[x] Two types of lighting with customizable colors and movement through GUI or ACTIONS<br>
[x] {ACTION_X} --- AFTER_M_SECONDS---Triggers---> {EVENT_A} ---> AFTER_N_SECONDS---Triggers---> {EVENT_B}

### Group A:

[ ] Frame-buffers with post-processing<br>
[ ] Off-screen Anti-Aliasing<br>
[ ] Parallax Mapping<br>
[x] Bloom with the use of HDR

### Group B:
[ ] Deferred Shading  
[x] Point Shadows  
[ ] SSAO

### Engine improvement:

[x] engine::graphics::Bloom — HDR framebuffer with MRT, ping-pong Gaussian blur, and a tone-mapped final composite pass<br>
[x] engine::graphics::Cube — generic primitive for emissive objects<br>
[x] engine::graphics::PointShadow — cubemap shadow map for point/spot light sources, using a geometry shader to render all 6 faces in a single pass

## Models:

[House model - PolyPizza](https://poly.pizza/m/bnZkUs4qEdG)<br>
[Streetlight - Sketchfab](https://sketchfab.com/3d-models/medellin-street-lamp-6cfedb57645b42b4a11d6c06411014b6)<br>
[Yellow car - PolyPizza](https://poly.pizza/m/5zUWP5UsLg-)<br>
[Taxi - PolyPizza](https://poly.pizza/m/fet47VieV0L)<br>
[Blue car - PolyPizza](https://poly.pizza/m/dggOiBLYyuR)<br>
Platform and road made in Blender - Jovan Mijajlović