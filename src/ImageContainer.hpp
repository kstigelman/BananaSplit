#include <iostream>
#include <SFML/Graphics.hpp>

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
  Image* source;
  Image* output;

  sf::Vector2u center;
  bool loaded;
  bool waitingOutput;
public:
  ImageContainer () {
    loaded = false;
    waitingOutput = false;
    source = nullptr;
    output = nullptr;
    extension = "png";
  }
  ~ImageContainer () {
    if (loaded)
      delete source;
    if (waitingOutput)
      delete output;
  }
  void generate (string fp) {
    if (loaded == true)
      delete source;

    filepath = fp;
    source = new Image ();

    if (source->loadFromFile (filepath)) {

      center = sf::Vector2u (source->getSize ().x / 2, source->getSize().y / 2);
      loaded = true;
    }
  }
  Image* getImage () {
    return source;
  }
  Image* getOutput () {
    return output;
  }
  Image* createNewOutput (int l, int h) {
    if (output != nullptr) {
      delete output;
      output = nullptr;
    }

    output = new Image ();
    waitingOutput = true;
    output->create (l, h);
    return output;
  }
  void setExtension (string ext) {
    extension = ext;
  }
  string getExtension () {
    return extension;
  }
  string getFilepath () {
    return filepath;
  }
  void close () {
    if (loaded)
      delete source;
    source = nullptr;
    if (waitingOutput)
      delete output;
    output = nullptr;
    waitingOutput = false;
    loaded = false;
    filepath = string();
  }
  bool save (string filename) {
    if (output == nullptr)
      return false;
    
    if (filename.find (".") != string::npos) {
      if (!output->saveToFile (filename))
        return false;
      printf ("Saved image to '%s'\n", filename.c_str());
    }
    else {
      string fullname = filename + "." + extension;
      if (!output->saveToFile (fullname))
        return false;
    }
    
    delete output;
    output = nullptr;
    waitingOutput = false;
    return true;
  }
  sf::Vector2u getCenter () {
    return center;
  }
  void setCenter (sf::Vector2u newCenter) {
    if (newCenter.x > source->getSize().x || newCenter.y > source->getSize().y)
      return;
    center = newCenter;
  }
  bool
  isLoaded () {
    return !loaded;
  }
};
