/*
  Project    : Banana Split
  Author     : Kyler Stigelman
  Description: A simple application to cut up images into boxes, or downscale an image.
 */

/*
 *******************************************************************************
 * INCLUDE DIRECTIVES
 *******************************************************************************
 */
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

#include "ImageContainer.hpp"
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


class NewImage {
private:
  string filepath;
  string extension;
  Image* source;
  Image* output;

  sf::Vector2u center;
  bool loaded;
public:
  NewImage () {
    loaded = false;
    source = nullptr;
    output = nullptr;
    extension = ".png";
  }
  ~NewImage () {
    delete source;
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
    delete source;
    source = nullptr;
    delete output;
    output = nullptr;
    filepath = string();
  }
  bool save (string filename) {
    if (output == nullptr)
      return false;
    
    if (filename.find (".")) {
      output->saveToFile (filename);
      printf ("Saved image to '%s'\n", filename.c_str());
    }
    else {
      string fullname = filename + "." + extension;
      output->saveToFile (fullname);
    }
    
    delete output;
    output = nullptr;
    
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

bool
isLoaded (ImageContainer image);

void
eval ();

void
center (ImageContainer& imageSource, sf::Vector2u center);

void
fail ();

bool 
checkImageLoaded (ImageContainer& img);

void
negate (ImageContainer& imageSource, int filter);

void
help();
void
compress (ImageContainer& imageSource, int cf);

void
split (ImageContainer& imageSource, int x, int y = 0, bool trunc = true);

void
dice (ImageContainer& imageSource, int x, int y = 0, bool trunc = true);

void
save (ImageContainer& imageSource, string filepath);

void
save (ImageContainer& imageSource);

void
setExtension (ImageContainer& imageSource, string type);

void
open (ImageContainer& imageSource, string filepath);

void
rotate (ImageContainer& imageSource, int direction);

void
crop (ImageContainer& imageSource, int width = 0, int height = 0);

void 
upscale (ImageContainer& imageSource, int scaleFactor = 2);

void
wild (ImageContainer& imageSource);

sf::Color 
add (sf::Color color, sf::Color color2);

int 
magnitude (sf::Color color);

sf::Color 
subtract (sf::Color color1, sf::Color color2);

int 
main ()
{
  eval ();
}

void
eval ()
{

  ImageContainer imageSource;

  string command;
  //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  // SetConsoleTextAttribute(hConsole, 14);
  printf ("\nWelcome to Banana Split\n\n");
  //SetConsoleTextAttribute(hConsole, 15);
  printf ("Type help for a list of commands.\n\nbspl > ");

  while (1)
  {
    string input;
    getline (cin, input);

    if (input.length() == 0) {
        printf ("bspl > ");
        continue;
    }
    vector<string> command;

    string temp;
    stringstream ss(input);
    while (getline(ss, temp, ' '))
      command.push_back(temp);

    size_t size = command.size();

    if (command.size () == 0)
      continue;
    
    if (command[0] == "help")
      help ();
    // open
    if (command[0] == "open") {
      if (command.size () >= 2) {
        open (imageSource, command[1]);
      }
      else {
        fail ();
      }
    }
    // compress
    if (command[0] == "compress") {
      
      if (command.size () >= 2) {
        compress (imageSource, std::stoi (command[1]));
      }
      else {
        fail ();
      }
    }
    // rotate
    if (command[0] == "rotate") {
      if (command[1] == "90")
        rotate(imageSource, 90);
      else if (command[1] == "-90")
        rotate (imageSource, -90);
    }
    // save
    if (command[0] == "save") {
      if (command.size () >= 2) {
        save (imageSource, command[1]);
      }
      else {
        fail ();
      }
    }
    if (command[0] == "center") {
      if (command.size () >= 3) {
        center (imageSource, sf::Vector2u (std::stoi (command[1]), std::stoi (command[2])));
      }
    }
    if (command[0] == "negate") {
      negate (imageSource, std::stoi (command[1]));
    }
    // split
    if (command[0] == "split") {
      if (command.size () >= 3) {
        if (command.size () >= 4 && command[3] == "trunc")
          split (imageSource, std::stoi (command[1]), std::stoi (command[2]), true);
        else
          split (imageSource, std::stoi (command[1]), std::stoi (command[2]), false);
      }
      else if (command.size () == 2) {
        split (imageSource, std::stoi (command[1]));
      }
      else {
        fail ();
      }
    }
    // crop
    if (command[0] == "crop") {
      if (command.size () >= 3)
        crop (imageSource, std::stoi (command[1]), std::stoi (command[2]));
      else if (command.size () == 2)
        crop (imageSource, std::stoi (command[1]));
      else
        crop (imageSource);
    }
    if (command[0] == "upscale") {
      if (command.size () >= 2) {
        upscale (imageSource, std::stoi (command[1]));
      }
      else {
        fail ();
      }
    } 
    // dice
    if (command[0] == "dice") {
      if (command.size () >= 4) {
        if (command[3] == "trunc")
          dice (imageSource, std::stoi (command[1]), std::stoi (command[2]), true);
        else
          dice (imageSource, std::stoi (command[1]), std::stoi (command[2]), false);
      }
      else if (command.size () == 3) {
        split (imageSource, std::stoi (command[1]), std::stoi (command[2]));
      }
      else if (command.size () == 2) {
        split (imageSource, std::stoi (command[1]));
      } else {
        fail ();
      }
    }
    if (command[0] == "wild") {
      wild (imageSource);
    }
    if (command[0] == "exit" || command[0] == "quit")
      return;

    printf ("bspl > ");
  }
}

void
open (ImageContainer& source, string filepath) {
  source.generate (filepath);
  printf ("Opened %s (%d, %d).\n", filepath.c_str(), source.getImage ()->getSize ().x, source.getImage ()->getSize ().y);
}
void
save (ImageContainer& source, string filepath) {
  if (!checkImageLoaded (source))
    return;
  if (!source.save (filepath))
    printf ("You haven't made any modifications to the image!\n");
}

void
save (ImageContainer& source) {
  if (!checkImageLoaded (source))
    return;
  string ext;
  printf ("Select a file extension type (default is 'png'): ");
  getline (cin, ext);
  setExtension (source, ext);

  string filename;
  printf ("Enter a name for the saved file:");
  getline (cin, filename);

  if (!source.save (filename))
    printf ("You haven't made any modifications to the image!\n");
}

bool
isLoaded (ImageContainer image) {
  return true;
}
void
fail () {
  printf ("\nError: Please use 'help' to review the commands!\n");
}
void
wild (ImageContainer& imageSource)
{
  if (!checkImageLoaded (imageSource))
    return;

  Image* input = imageSource.getImage (); 
  
	int l = input->getSize().x;
	int h = input->getSize().y;

  Image* output = imageSource.createNewOutput (l, h);
  
  int cf = 100;
  int cf2 = 100;

	int trueI = 0;
	int trueJ = 0;
	
	
	sf::Color base =sf::Color (45, 57, 32);
	int baseMag = magnitude (base);

	int tolerance = 30;
	
	int rt = 30;
	int gt = 30;
	int bt = 30;
	for (int j = 0; j < h; ++j)
	{
		for (int i = 0; i < l; ++i) 
		{
			
      if (i < 1900 && j > 1800 && j < 2600) {
        if (i + std::cos (i) * 2 < 0 || j +std::sin (j) * 6 < 0) {
          output->setPixel (int (i + std::cos(i * 0.5) * 2), int (j + std::sin (j * 0.5) * 6), add (input->getPixel (i, j), sf::Color (40, 60, 10))) ;
        }
        else {
          output->setPixel (i, j, add (input->getPixel (i, j), sf::Color (22, 100, 32))) ;
        }


      } else {
        output->setPixel (i, j, input->getPixel (i, j)) ;
      }
			/*if (magnitude (input->getPixel(i, j)) < baseMag + tolerance &&
			    magnitude (input->getPixel(i, j)) > baseMag - tolerance)
			{
				if (input->getPixel (i, j).r < base.r + rt &&  input->getPixel (i, j).r > base.r - rt &&
					input->getPixel (i, j).g < base.g + gt &&  input->getPixel (i, j).g > base.g - gt &&
				    input->getPixel (i, j).b < base.b + bt &&  input->getPixel (i, j).b > base.b - bt)
			
				output->setPixel (i, j, sf::Color (input->getPixel(i, j).r, input->getPixel(i, j).g, input->getPixel(i, j).b));
				//else
					//cOutput.setPixel (i, j, sf::Color (input.getPixel(i, j).r, input.getPixel(i, j).g, input.getPixel(i, j).b));
			}
			else
			{
				output->setPixel (i, j, sf::Color (input->getPixel(i, j).r, input->getPixel(i, j).g, input->getPixel(i, j).b));
			}
			if (i % 3 == 0)
			{
				output->setPixel (i, j, sf::Color (input->getPixel(i, j).r, input->getPixel(i, j).g, input->getPixel(i, j).b));
			}
			if (i % 7 == 0)
			{
				//if (h - j > 0) 
				//		cOutput.setPixel (i, h - j, sf::Color (input.getPixel(i, j).r, input.getPixel(i, j).b, input.getPixel(i, j).g));
			}*/
		}
	}

 
}
void
compress (ImageContainer& imageSource, int cf) 
{
  if (!checkImageLoaded (imageSource))
    return;

  if (cf < 0) {
    printf ("\033[31mError: Compression factor must not be negative!\033[0m\n");
    return;
  }

  Image* input = imageSource.getImage (); 
  
	int l = input->getSize().x;
	int h = input->getSize().y;
	
  Image* output = imageSource.createNewOutput (l / cf, h / cf);

  std::printf ("Compressing image (%d, %d) down to %d x %d.\n", l, h, l / cf, h / cf);
	
	for (int i = 0; i < l / cf; i++)
	{
		for (int j = 0; j < h / cf; j++)
		{
			Color temp = input->getPixel(i * cf, j * cf) + input->getPixel(i * cf + 1, j * cf) + input->getPixel(i * cf, j * cf + 1) + input->getPixel(i * cf + 1, j * cf + 1);
			int values[3] = { input->getPixel(i * cf, j * cf).r + input->getPixel(i * cf + 1, j * cf).r + input->getPixel(i * cf, j * cf + 1).r + input->getPixel(i * cf + 1, j * cf + 1).r,
							  input->getPixel(i * cf, j * cf).g + input->getPixel(i * cf + 1, j * cf).g + input->getPixel(i * cf, j * cf + 1).g + input->getPixel(i * cf + 1, j * cf + 1).g,
							  input->getPixel(i * cf, j * cf).b + input->getPixel(i * cf + 1, j * cf).b + input->getPixel(i * cf, j * cf + 1).b + input->getPixel(i * cf + 1, j * cf + 1).b };
			output->setPixel(i, j, Color(values[0] / 4, values[1] / 4, values[2] / 4));
		}
	}
}
void
upscale (ImageContainer& imageSource, int scaleFactor) {
  if (!checkImageLoaded (imageSource))
    return;

  if (scaleFactor <= 0) {
    printf ("\033[31mError: Scale factor must be greater than 0!\033[0m\n");
    return;
  }

  Image* input = imageSource.getImage ();
  int sl = input->getSize().x * scaleFactor;
  int sh = input->getSize().y * scaleFactor;
  Image* output = imageSource.createNewOutput (sl, sh);

  for (int i = 0; i < sl; ++i) {
    for (int j = 0; j < sh; ++j) {
      output->setPixel (i, j, input->getPixel (i / scaleFactor, j / scaleFactor));
    }
  }
}
void
split (ImageContainer& imageSource, int x, int y, bool trunc) {
  if (!checkImageLoaded (imageSource))
    return;
  if (x < 0 || y < 0) {
    printf ("\033[31mError: x and y must not be negative!\033[0m\n");
    return;
  }

  Image* input = imageSource.getImage ();

	int px = input->getSize().x / x;
  int py = input->getSize().y / x;
  
  if (y != 0)
    py = input->getSize().y / y;
  
  printf ("Split file into %d x %d boxes.\n", x, (px == py ? x : y));

	Image output;
  output.create (px, py);
	
	int remainderX = input->getSize().x % px;
	int remainderY = input->getSize().y % py;

	int width = (input->getSize().x - remainderX) / px;
	int height = (input->getSize().y - remainderY) / py;
	
  printf ("(%d %d)\n", width, height);
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			output.copy(*input, 0, 0, IntRect(i * px, j * py, px, py), false);
			output.saveToFile("data/" + to_string(i + j * width) + ".png");
		}
	}
}
void
rotate (ImageContainer& imageSource, int direction) {
  if (!checkImageLoaded (imageSource))
    return;

  if (direction % 90 != 0) {
    printf ("\033[31mError: Can only rotate in a multiple of 90 degrees!\033[0m\n");
    return;
  }

  Image* input = imageSource.getImage ();

  int l = input->getSize().x;
	int h = input->getSize().y;
	
  Image* output;

  if (direction == 180 || direction == -180) {
    output = imageSource.createNewOutput (l, h);

    for (int j = 0; j < h; ++j)
        for (int i = 0; i < l; ++i)
          output->setPixel (l - i - 1, h - j - i, input->getPixel(i,j));

    return;
  }

  output = imageSource.createNewOutput (h, l);

  if (direction == 90 || direction == -270) {
    for (int j = 0; j < h; ++j)
        for (int i = 0; i < l; ++i)
          output->setPixel (h - j - 1, i, input->getPixel(i,j));
  }
  if (direction == -90 || direction == 270) {
    for (int j = 0; j < h; ++j)
        for (int i = 0; i < l; ++i)
          output->setPixel (j, l - i - 1, input->getPixel(i,j));
  }
}

void
crop (ImageContainer& imageSource, int width, int height) {
  if (!checkImageLoaded (imageSource))
    return;

  if (width < 0 || height < 0) {
    printf ("\033[31mError: Width and height can not be negative!\033[0m\n");
    return;
  }

  Image* input = imageSource.getImage ();
  sf::Vector2u size = input->getSize();
  sf::Vector2u center = imageSource.getCenter ();


  if (width > size.x || height > size.y) {
    printf ("Error: Provided dimensions are too large!");
    return;
  }

  int x_span, y_span, left, top;

  x_span = width;
  y_span = height;

  if (width == 0) {
    if (size.x >= size.y) {
      x_span = size.y;
      y_span = size.y;
    }
    else {
      x_span = size.x;
      y_span = size.x;
    }   
  }
  else if (height == 0) {
    y_span = width;
  }

  left = center.x - (x_span / 2);
  top = center.y - (y_span / 2);

  if (left < 0)
    left = 0;
  else if (center.x + (x_span / 2) > size.x)
    left = size.x - x_span;
  if (top < 0)
    top = 0;
  else if (center.y + (y_span / 2) > size.y)
    top = size.y - y_span;
  
  Image* output = imageSource.createNewOutput (x_span, y_span);

  for (int j = 0; j < y_span; ++j)
    for (int i = 0; i < x_span; ++i)
      output->setPixel (i, j, input->getPixel (i + left, j + top));

  /*
  

  x_width = width;
  y_width = height;

  if (width == 0) {
    if (height != 0)
      x_width = y_width;
  }
  else if (height == 0) {
    y_width = x_width;
  }
  else {
      if (input->getSize().x >= input->getSize().y) {
        x_width = input->getSize().y;
        y_width = input->getSize().y;
      }
      else {
        x_width = input->getSize().x;
        y_width = input->getSize().x;
      }   
  }

  left = center.x - (width / 2);
  if (left < 0)
    left = 0;
  top = center.y - (height / 2);
  if (top < 0)
    top = 0;

  if (center.y + (height / 2) > input->getSize().y)
    y_width = input->getSize().y - 1;

  if (center.x + (width / 2) > input->getSize().x)
    x_width = input->getSize().x - 1;

  Image* output = imageSource.createNewOutput (x_width, y_width);

  for (int j = 0; j < y_width; ++j) {
    for (int i = 0; i < x_width; ++i) {
      output->setPixel (i, j, input->getPixel (i + left, j + top));
    }
  }*/
  //output->copy(*input, 0, 0, IntRect(top, left, x_width, y_width), true);
}

void
dice (ImageContainer& imageSource, int x, int y, bool trunc) {
  if (!checkImageLoaded (imageSource))
    return;

  if (x < 0 || y < 0) {
    printf ("\033[31mError: x and y can not be negative!\033[0m\n");
    return;
  }
  Image* input = imageSource.getImage ();

	int px = input->getSize().x / x;
  int py = input->getSize().y / x;
  
  if (y != 0)
    py = input->getSize().y / y;
  
  printf ("Splitting file into %d x %d boxes.\n", x, (px == py ? x : y));

	Image output;
  output.create (px, py);
	
  if (trunc) {
    int remainderX = input->getSize().x % px;
	  int remainderY = input->getSize().y % py;

	  int width = (input->getSize().x - remainderX) / px;
	  int height = (input->getSize().y - remainderY) / py;
	
    printf ("(%d %d)\n", width, height);
	  for (int i = 0; i < width; ++i)
	  {
		  for (int j = 0; j < height; ++j)
		  {
			  output.copy(*input, 0, 0, IntRect(i * px, j * py, px, py), false);
			  output.saveToFile("data/" + to_string(i + j * width) + ".png");
		  }
	  }
	  printf ("Splitting completed.\n");
  }
	else {
    int width = input->getSize().x / px;
	  int height = input->getSize().y / py;
	
    printf ("(%d %d)\n", width, height);
	  for (int i = 0; i < width; ++i)
	  {
		  for (int j = 0; j < height; ++j)
		  {
			  output.copy(*input, 0, 0, IntRect(i * px, j * py, px, py), false);
			  output.saveToFile("data/" + to_string(i + j * width) + ".png");
		  }
	  }
	  printf ("Splitting completed.\n");
  }
}

void
center (ImageContainer& imageSource, sf::Vector2u center) {
  if (!checkImageLoaded (imageSource))
    return;
  imageSource.setCenter (center);
}

void
negate (ImageContainer& imageSource, int filter) {
  if (!checkImageLoaded (imageSource))
    return;

  Image* input = imageSource.getImage ();
  Image* output = imageSource.createNewOutput (input->getSize().x, input->getSize().y);

  for (int j  = 0; j < output->getSize().y; ++j) { 
    for (int i = 0; i < output->getSize().x; ++i) {
      sf::Color color = input->getPixel (i, j);
      if (color.r + color.g + color.b >= filter)
        output->setPixel (i, j, sf::Color::White);
      else
        output->setPixel (i, j, sf::Color::Black);
    }
  }

}
void
help () {
  printf ("open <filepath>         ===>  Load an image for editing.\n\n");
  printf ("compress <factor>       ===>  Downscale image by a given factor. Caches image in memory.\n\n");
  printf ("split <X> <*Y> <*trunc> ===>  Split an image into X by Y boxes.\n");
  printf ("                              *Y parameter is optional. If empty, will use X.\n\n");
  printf ("                              *trunc parameter is optional. If present, excess pixels will get truncated.\n\n");
  printf ("                              *trunc is always true if Y is not provided.\n\n");
  printf ("rotate <90/-90>         ===>  Rotate the image 90 degrees clockwise or counter-clockwise.\n");
  printf ("crop <w> <*h>           ===>  Crops the image from the center to be of dimensions w x h (width x height).\n");
  printf ("                              *h parameter is optional. If empty, will crop as a square of size w.\n\n");
  printf ("center <X> <Y>          ===>  Set the center of the image at a given pixel location. Operations such as crop,\n\n");
  printf ("                              which truncates pixels, will differ depending upon the skew of the center.\n\n");
  printf ("upscale <factor>        ===>  Upscales the image dimensions by a given factor.\n\n");
  printf ("negate <threshold>      ===>  Creates a black and white image. Intensity of image will depend on the \"threshold value\"\n\n");
  printf ("                              which each pixel is compared to (Recommended: 180 - 250).\n\n");
  printf ("save <filepath>         ===>  Save cached image to a given location.\n\n");
  printf ("close                   ===>  Close the currently open image file.\n\n");
  printf ("quit                    ===>  Exit the application.\n\n");
}

bool checkImageLoaded (ImageContainer& img) {
  if (img.getImage () == nullptr) {
    printf ("\033[31mError: No image has been loaded!\033[0m\n");
    return false;
  }
  return true;
}

void
setExtension (ImageContainer& imageSource, string type) {
  imageSource.setExtension (type);
}

sf::Color 
add (sf::Color color1, sf::Color color2)
{
	return sf::Color (color1.r + color2.r, color1.g + color2.g, color1.b + color2.b);
	
}
int magnitude (sf::Color color)
{
	float r = std::pow (color.r, 2);
	float g = std::pow (color.g, 2);
	float b = std::pow (color.b, 2);
	
	return std::sqrt(r + g + b);
}
sf::Color subtract (sf::Color color1, sf::Color color2)
{
	return sf::Color (color1.r - color2.r, color1.g - color2.g, color1.b - color2.b);
	
}