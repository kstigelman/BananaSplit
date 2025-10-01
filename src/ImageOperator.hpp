#pragma once

#include <string.h>

class ImageOperator {
    private:
        class ImageContainer* m_image;
    
    public:
        ImageOperator() { 

        }
        ImageOperator (ImageContainer* image) {
            m_image = image;
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
};