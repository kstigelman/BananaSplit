/*if (!checkImageLoaded (imageSource))
    return;

  Image* input = imageSource.getImage (); 
  
	int l = input->getSize().x;
	int h = input->getSize().y;

  Image* output = imageSource.createNewOutput (l, h);
  
  int cf = 100;
  int cf2 = 100;

	int trueI = 0;
	int trueJ = 0;
	
	
	sf::Color base =sf::Color (3, 57, 230);
	int baseMag = magnitude (base);

	int tolerance = 84;
	
	int rt = 35;
	int gt = 40;
	int bt = 47;
	for (int j = 0; j < h; ++j)
	{
		for (int i = 0; i < l; ++i) 
		{
			
			if (magnitude (input->getPixel(i, j)) < baseMag + tolerance &&
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
			}
		}
	}
  

  int trueI = 0;
  int trueJ = 0;
  for (int j = 1; j < l - 1; j++)
	{
		for (int i = 1; i < h - 1; i++)
		{
			if (i % 2 == 0 && i + 1 < output->getSize().x && j + 1 < output->getSize(). y)
			{
				int avgR = (input->getPixel(i, j).r + input->getPixel(i + 1, j).r + 
				           input->getPixel(i, j + 1).r + input->getPixel(i + 1, j + 1).r) / 4;
				int avgG = (input->getPixel(i, j).g + input->getPixel(i + 1, j).g + 
				           input->getPixel(i, j + 1).g + input->getPixel(i + 1, j + 1).g) / 4;
				int avgB = (input->getPixel(i, j).b + input->getPixel(i + 1, j).b + 
				           input->getPixel(i, j + 1).b + input->getPixel(i + 1, j + 1).b) / 4;
				           
				output->setPixel (trueI, trueJ, sf::Color(avgR, avgB, avgG));
				++trueI;
			}
		}
		if (j % 2 == 0)
			++trueJ;
	}*/
/*
  int rowSum = 0;
	int columnSum = 0;
	for (int j = 0; j < h; ++j)
	{
		rowSum = 0;
		for (int i = 0; i < l; ++i)
			rowSum += magnitude (input->getPixel(i, j));
		rowSum = rowSum / l;
		columnSum += rowSum;
	}
	int average = columnSum / h;
	
	for (int j = 0; j < h; ++j)
	{
		for (int i = 0; i < l; ++i) 
		{
			if (magnitude (input->getPixel (i, j)) > average)
				output->setPixel (i, j, add (input->getPixel(i, j), subtract (input->getPixel(i, j), sf::Color (average, average, average))));
			else
				output->setPixel (i, j, subtract (sf::Color (average, average, average), input->getPixel(i, j)));
		}
	}*/
/*
  int amp = h / 1;
	int freq = l / 1;
	
	int iVal = 120;
	int iMax = 701;

  Image* output = imageSource.createNewOutput (l, h);

	for (int j = 0; j < h; ++j)
	{
		for (int i = 0; i < l; ++i) 
		{
			output->setPixel (i, j, input->getPixel(i, j));
		}
	}
	int c = 0;
	int kVal = 50;
	
	for (int j = 93; j < h; j += 266)
	{
		if (c % 2 == 0)
		{
			kVal += 27;
			for (int k = 0; k < kVal; ++k)
      {
        for (int i = iVal; i < iMax; ++i) 
        {
          if (j + k > h)
            continue;
          output->setPixel (i, j + k, add (input->getPixel(i, j), sf::Color (30, 70, 40)));
        }
      }
		}
		else
		{
      for (int k = 0; k < 50; ++k)
      {
        for (int i = iVal; i < iMax; ++i) 
        {
          if (j + k > h)
            continue;
          output->setPixel (i, j + k, add (input->getPixel(i, j), sf::Color (30, 70, 40)));
          
        }
      }
      
		}
	  ++c;
  }*/
  /*
  

  for (int i = 0; i < l / cf; i++)
	{
		for (int j = 0; j < h / cf; j++)
		{




    }
  }*/