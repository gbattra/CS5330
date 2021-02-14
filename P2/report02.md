

### Color & Texture

**Without Color**
- show results for pic.0035.jpg <- good results that generally match the texture of the original image
- show results for pic.0035.jpg with just color histo <- ok results but not as good as with texture feature included
- show results for pic.0535.jpg <- bad results which have no correlation with one another
I used pic.0035.jpg as a baseline to see if the gms texture filter even really worked. in an optimal scenario it worked,
so I figured the bad results with other images was due to the fact that summing up gradient magnitudes wasn't
sophisticated enough for complex images.

**With Color**
- we see better results but still not great. if we remove the texture distance from overall distance metric, we get roughly the
same results. which indicates that the color histogram determines the lion's share of the distance between two images

### Part 5: Custom Feature Vector
**Feature Vector**

For this part, I chose to implement a feature vector consisting of a texture feature vector using Laws filters and a color feature vector of the Red/Green space of the image. The Laws filters were applied over a 100 x 100 centered slice of the image, while the R/G feature vector was built over the whole image.

I used the following combinations of Laws filters for the texture features:
- Gaussian x Spot / Spot x Gaussian
- Gaussian x Derivative / Derivative x Gaussian
- Wave x Ripple / Ripple x Wave

The response of each were normalized of the reponse of the 5x5 Gaussian filter.

**Metric**

To measure the distance between two of these feature vectors, I split the concatenated vector into the Laws texture features and Red/Greed features respectively. I used Sum of Squared Distance for the texture features and Intersection for the Red/Green features. I weighed the texture distance by 75% and the Red/Green distance by %25. This assumes that image content is primarily described by texture and not color, which may not always be the case (i.e. banana slices and kiwi slices have the same texture, but different color).

**Testing**

I tested this architecture using the same target image over two sets of data: `test` and `db`. `test` contained a small numer (~30) images, while `db` contained ~1000 images. However, the same images in `test` were also in `db` as a control.

Below are the results for the `test` dataset, in order of best match to worst match:

**Target Image**
<br>
<img src="images/examples/part5/target.png" width="300px"/>

**Results**
<br>
<img src="images/examples/part5/test/1.png" width="200px"/>
<img src="images/examples/part5/test/2.png" width="200px"/>
<img src="images/examples/part5/test/3.png" width="200px"/>
<img src="images/examples/part5/test/4.png" width="200px"/>
<img src="images/examples/part5/test/5.png" width="200px"/>
<img src="images/examples/part5/test/6.png" width="200px"/>
<img src="images/examples/part5/test/7.png" width="200px"/>
<img src="images/examples/part5/test/8.png" width="200px"/>
<img src="images/examples/part5/test/9.png" width="200px"/>
<img src="images/examples/part5/test/10.png" width="200px"/>

The results here are pretty good. We have images with matching texture, and besides the grayscale image, roughly the same color. There are a few errant results, such as the fruit box and the pen. This could be due to the fact that the feature vector is sampled from the center 100x100 pixel section of the image.

Here are the results of the `db` dataset:

**Target Image**
<br>
<img src="images/examples/part5/target.png" width="300px"/>

**Results**
<br>
<img src="images/examples/part5/db/1.png" width="200px"/>
<img src="images/examples/part5/db/2.png" width="200px"/>
<img src="images/examples/part5/db/3.png" width="200px"/>
<img src="images/examples/part5/db/4.png" width="200px"/>
<img src="images/examples/part5/db/5.png" width="200px"/>
<img src="images/examples/part5/db/6.png" width="200px"/>
<img src="images/examples/part5/db/7.png" width="200px"/>
<img src="images/examples/part5/db/8.png" width="200px"/>
<img src="images/examples/part5/db/9.png" width="200px"/>
<img src="images/examples/part5/db/10.png" width="200px"/>

As you can tell, the results here are not as good. However, we can still see some patterns in texture, and the majority of the images contain brick walls, like the target image. Not a complete failure, but definitely leaves room for improvement. In the `Extensions` sections below, I expand upon this initial architecture and produce much better results.

### Extensions

**Sliding Laws Filter**