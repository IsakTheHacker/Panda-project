//Colors
#define color_blue 1;
#define color_green 2;
#define color_aqua 3;
#define color_red 4;
#define color_purple 5;
#define color_yellow 6;
#define color_white 7;
#define color_gray 8;
#define color_lightblue 9;
#define color_black 0;
#define color_lightgreen "A";
#define color_lightaqua "B";
#define color_lightred "C";
#define color_lightpurple "D";
#define color_lightyellow "E";
#define color_brightwhite "F";

<Texture> initialShadingGroup {
  "tunnel.jpg"
  <Scalar> format { rgb }
  <Scalar> wrapu { repeat }
  <Scalar> wrapv { repeat }
  <Scalar> minfilter { linear_mipmap_linear }
  <Scalar> magfilter { linear }
}