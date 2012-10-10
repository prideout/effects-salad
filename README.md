sequence as of Oct 1
====================
 - 17s Empty Grassy Field, Wind Blowing, Highlight Fireflies, Moon
 - 30s Beats enter, Buildings poppin' up but without detail
 - 30s Grassy field, Tree starts growing
 - 30s Buildings, detail starts popping in, camera crawls up buildings, zooming on detail
 - 30s Blossoms growing and falling
 - 20s Back to city -- explosion and camera shake?
 - 10s Credits (music keeps going)

rideout queue
=============
 - ~~submerge the building "seeds"~~
 - ~~land the windows branch~~
 - ~~split the CityGrow shot in two~~
 - ~~Use the swanky new music API~~
 - ~~add viewing angle to the hand-coded "script" to prevent blocked views~~
 - ~~add building details: inset windows, antenna, roof extensions~~
 - ~~SSAO~~
 - ~~Perturb rows in GridCity~~
 - Pop in buildings in GridCity from center out (tendrils?) -- use hihats for fast popping!
 - Detail per the reference image
 - PUNT unify the two cities in some way
 - PUNT halfway into CityGrow, pull back the camera and start popping buildings once per beat
 - transform-feedback curl noise for sand
 - laser beams for city explosions
 - credits 
 - make the blank terrain more interesting
 - city layout

cowles queue
============
 - ~~music API~~
 - ~~finish leaf shading~~
 - ~~finish leaf lighting~~ (done for now, might need to revisit)
 - ~~make grass blow in the wind~~ (done for now, need to revisit)
 - ~~make leaves fall and float in wind~~
 - ~~split GrassIntro into 3 shots~~
 - ~~make fireflies blink~~
 - growth camera work
 - leaf keep-alive movement
 - move fireflies to music or flocking
 - moon + moon effect
 - screen space effects (bloom, color separation, fake dof)
 - city sky
 - tree bark shading
 - horizontal vine effect / floral flourish
 - credits
 - linux & mac fullscreen mode switch
 - statically link SDL & SDL_mixer


effects-salad
=============

1) Go to lib/jsoncpp and run ./setup.sh

2) Go to lib/tetgen and run "make tetlib"

3) Setup SDL


> **For OSX:**
>
> http://www.libsdl.org/release/SDL-1.2.15.dmg
>
> http://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-1.2.12.dmg
>
> Copy SDL.framework and SDL_mixer.framework into /Library/Frameworks
>
>**For Linux:**
>
>sudo yum install SDL_mixer-devel

Inspiration
-----------
Videos:
  * [Sol 740 blowing off Tetsuo's arm](http://www.youtube.com/watch?feature=player_detailpage&v=LALsuMWv2ps#t=178s)
  * [Many Viewports from 'Life on Mars'](http://www.youtube.com/watch?v=yIMP6-KBSCs#t=17s)
  * [Walking through 5ft grass](http://www.youtube.com/watch?feature=player_detailpage&v=y0AexwPTz1k#t=40s)
  * [Grass Sound](http://www.youtube.com/watch?v=ldGw2mJA5_o&feature=related)
  * [Journey through tall grass](http://www.youtube.com/watch?v=Hsw0trAkKuE&feature=related)
  * [First person view walking](http://www.youtube.com/watch?feature=player_detailpage&v=LYZeB44xrow#t=35s)

Stills:
  * [Knot Tubes](http://www.originalsoundversion.com/wp-content/uploads/2008/12/badloop_luo.jpg)
  * http://minivegas.net/wp-content/uploads/2012/07/demoscene-moleman-art-algorithm_original_original.jpg
  * http://www.marklazenby.co.uk/files/mark-lazenby-make-mountains-5.jpg
  * http://farm8.staticflickr.com/7110/7692526148_d7cf675343_z.jpg
  * http://25.media.tumblr.com/tumblr_m0g3jiGyo21qa6hruo1_500.jpg
  * http://farm3.static.flickr.com/2475/3916967801_da152c16f7_b.jpg
  * http://farm3.static.flickr.com/2789/4333057631_0e1070bdfa_o.jpg

City:
  * [Yemen Cube City](http://ameralwarea.files.wordpress.com/2010/02/20080426mudbrick-houses-shibam-yemen-20052.jpg)
  * [City Sculpture](http://www.talariaenterprises.com/images2/5089a.jpg)
  * [NYC Google Earth](http://sites.duke.edu/tlge/files/2010/02/google_earth_nyc_801.jpg)
  * [Sky scraper - sense of scale](http://www.rianicpa.com/images/resources_buildings.jpg)
  * [City art floral flourish](http://image.shutterstock.com/display_pic_with_logo/95809/95809,1256699441,3/stock-vector-black-city-buildings-and-graffiti-grunge-floral-arrow-design-39726994.jpg)

Cherry Blossom / Grass / Country / moon:
  * [Blossom up close](http://www.onlyinap.com/wp-content/uploads/2011/04/Cherry-Blossom-sprig1.jpg)
  * [Blossoms & Bark, huge](http://www.danielhaydenberman.com/wp-content/images/Wallpaper%20Wednesday/week_13/cherry-tree-1920x1200.jpg)
  * [Cool sky and moon](http://www.flickr.com/photos/25259860/2403305820/)
  * [Koi](http://www.flickr.com/photos/jpinlac/4572623911/)
  * [Blossoms on water](http://www.flickr.com/photos/shanmoe/3529404508/)
  * [Moon Ripples](http://www.flickr.com/photos/allybeag/2277580669/)
  * [Moon](http://www.flickr.com/photos/penguinbush/2768719983/)
  * [Moon glow through branches](http://www.flickr.com/photos/35077273@N06/4312923500/)
  * [Norway Northern Lights](http://www.youtube.com/watch?v=izYiDDt6d8s)
  * [Milkyway not great](http://www.flickr.com/photos/37916495@N07/3667064453/)
  * [Milkyway vignette](http://www.flickr.com/photos/41539320@N08/7289245018/)
  * [Milkyway and bail](http://www.flickr.com/photos/blakesmithphotography/5489197061/)
  * [Milkyway at Arosa](http://www.flickr.com/photos/reflection-stalker/7809105816/)

Cherry Tree Structure:
  * [Tree branches under](http://singlemomwithlove.com/wp-content/uploads/2011/12/Cherry-Trees.jpg)
  * [Tree far away](http://vikneshan.files.wordpress.com/2008/09/cherrytree.jpg)
  * [Painted/Artistic](http://lucien0maverick.files.wordpress.com/2012/08/cherry-tree-by-alex-khrapko.jpg)
  * [Multiple trunks](http://westofpersia.files.wordpress.com/2010/04/bbg-garden-cherry-tree-path.jpg)

Misc. Tree Art:
  * [Moon Haze](http://fc08.deviantart.net/fs50/i/2009/258/e/f/moon_haze_by_CharlotteDeSmith.jpg)
  * [Cool b/w silhouette](http://i.istockimg.com/file_thumbview_approve/11177570/2/stock-photo-11177570-black-birch-betula-pendula-tree-on-hill-isolated-o_n-white.jpg)

Torii:
  * [Torii in water](http://www.kuffner.org/james/gallery/raytracing/torii/Miyajima_torii_gate_postcard.jpg)

Neo-Tokyo:
  * http://www.madmind.de/wp-content/uploads/2010/03/akira-neo-tokyo.jpg
  * http://2.bp.blogspot.com/-KW_k5R8wWis/TePH6x3de9I/AAAAAAAAAms/3POananT2ds/s1600/Akira_city.jpg
  * http://dessinsanimes.canalblog.com/images/akira6.jpg
  * http://media.animevice.com/uploads/1/16037/268170-akira06_super.jpg
  * http://www.fanboyreview.net/wp-content/uploads/2009/01/akira-bd-cap001.png

Interactive:
  * http://phong.com/astro/hexasis/

Possible Sound + Music Stuff
----------------------------
  * [libsndfile](http://www.mega-nerd.com/libsndfile)
  * [portaudio](http://www.portaudio.com)
  * [flstudio](http://www.image-line.com/documents/flstudio.html)

