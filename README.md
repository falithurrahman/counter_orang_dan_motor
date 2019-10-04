### Description
In this project i made a program to count the number of people or motorbike that passed the frame. I used the technique named background subsctraction for movement detection in this project. The code and video source is available at this repository. I will briefly explain the code for you through flowchart below.

![flowchart](https://github.com/falithurrahman/people_and_bike_counter/blob/master/flowchart.jpg)

First step of all, i load the video used in this project. I took this video at the parking space of my college. You will see motorbike and people pass by inside the frame. Next i convert the video to grayscale and perform gaussian blur. It will ease us at processing the video later on. At this rate, i've done the easy part because next one is a little bit more difficult.

This step is the background subsctraction part, you can see the code from line 38 to 47. Background substraction is purposed to detect object movement or where the exact location of the moving object is. The idea is quite simple, *we find the difference inside the frame which is taken at the same place but with different timestamp.* Now look at the code, 

```C++
		it0 = gray.clone();
		if (cnt == 0) {
			it1 = it0.clone();
			cnt = 1;
			hasil = it0.clone();
		}

		absdiff(it0, it1, hasil);

		it1 = it0.clone();
```