

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