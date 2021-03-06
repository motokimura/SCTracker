SCTracker
===
A library to track spacecraft, written in C/C++.

## Description
This library enables you to calculate the position and velocity of spacecraft launched at the Earth. 
The direction (azimuth and elevation) to the spacecraft and doppler rate at your location can be also calculated 
so that you can use this for spacecraft tracking at your ground station.

## Usage
A sample program is written in main.cpp. The orbit parameters defined as Cartesian elements in the sample are adjusted to ones of [ARTSAT](http://artsat.jp) space-probe, [DESPATCH](http://artsat.jp/en/project/despatch). To track another spacecraft, specify its Cartesian elements using setSpacecraftInfo( ) function that is defined in tracker.cpp.

## Projects
[DESPATCH Orbit 3D Viewer](http://artsat.jp/en/project/despatch/orbit)

[DESPATCH in Starlit Sky](http://artsat.jp/en/project/despatch/celestial)

[DESPATCH Tracking API](http://api.artsat.jp/pass/)

## License

[MIT License](LICENSE.txt)

## Team

[Motoki Kimura](https://github.com/motokimura)

[ARTSAT Project](https://github.com/ARTSAT)
