#include <iostream>
#include "SFML/Graphics/Image.hpp"
#include "SFML/System/Vector2.hpp"

// For printf
#include <stdio.h>

#include <string.h>

#include <random>
#include <ctime>
#include <vector>
#include <queue>
#include <sstream>
//#include <windows.h>

using std::cout;
using std::string;
using std::to_string;
using std::srand;
using std::rand;
using std::vector;
using std::queue;
using std::stringstream;
using std::cin;

using sf::Image;
using sf::IntRect;
using sf::Color;

class ImageContainer {
private:
  string filepath;
  string extension;
  Image source;
  Image output;

  sf::Vector2u center;
  bool loaded;
  bool hasOutput;
public:
  ImageContainer () {
    loaded = false;
    hasOutput = false;
    extension = ".png";
  }
  ~ImageContainer () {
  }
  void generate (string fp) {
    filepath = fp;

    if (source.loadFromFile (filepath)) {
      center = sf::Vector2u (source.getSize ().x / 2, source.getSize().y / 2);
      loaded = true;
    }
  }
  Image* getImage () {
    if (!loaded)
      return nullptr;
    return &source;
  }
  Image* getOutput () {
    if (!hasOutput)
      return nullptr;
    return &output;
  }
  Image* createNewOutput (int l, int h) {
    if (!loaded)
      return nullptr;

    output.create (l, h);
    hasOutput = true;
    return &output;
  }
  void setExtension (string ext) {
    extension = ext;
  }
  string getExtension () {
    return extension;
  }
  string getFilepath () {
    if (!loaded)
      return "";
    return filepath;
  }
  void close () {
    loaded = false;
    hasOutput = false;
    filepath = string();
  }
  bool save (string filename) {
    if (!hasOutput)
      return false;
    
    if (filename.find (".") != string::npos) {
      if (!output.saveToFile (filename))
        return false;

      printf ("Saved image to '%s'\n", filename.c_str());
    }
    else {
      string fullname = filename + "." + extension;
      if (!output.saveToFile (fullname))
        return false;
    }
    
    return true;
  }
  sf::Vector2u getCenter () {
    return center;
  }
  void setCenter (sf::Vector2u newCenter) {
    if (newCenter.x > source.getSize().x || newCenter.y > source.getSize().y)
      return;
    center = newCenter;
  }
  bool
  isLoaded () {
    return !loaded;
  }
};
