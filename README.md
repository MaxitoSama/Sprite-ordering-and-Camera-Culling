# First things firts:

I am  [MARC GARCIA](www.linkedin.com/in/marc-garcia-93ab96145), student of the [Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/ing/estudis/graus-videojocs/). This content is generated for the second year’s subject Project 2, under supervision of lecturer [Ricard Pillosu](https://es.linkedin.com/in/ricardpillosu).

---

# Sprite Ordering and Camera Culling

In this Github page you will find all the information related with Sprite Ordering and Camera Culling in order to learn how to apply it in your code. You will learn the management of Priority Queues, collision with the camera and an optimization of collision calculation using Quadtrees. 

The first question that you maybe have in your mind is "Why I need that? What's wrong with my code?" but let me answer you with another questions:

* Do you want to change all your code every time you want to print something in a different order or you prefer a faster method?
* It's necessary to print every particle, entity or tile that isn't seen on the screen?
* If I have two entities, one in the right side of my map and the other on the opposite side, do you want to check if they are going to colide? What if you have thousands of entities?
* This is the most important question, Do you want that your game (or program) runs faster?

Now you that you know that it's always important to optimize your code, let me give you a guide focus on how to improve your code on these three topics.

## Sprite Ordering

I've said before that it's important to have a god way to manage the order of the sprites, but what I mean with sprite ordering? 
Well, in order to explain that first you have to know how a game randers the image.

The order of the images in the screen will appear in the inverse order that we call the our function that prints or render the image. This means that the first image we have sent to print will be the first image to be rendered, so it will be in the background. The secon one will be rendered next to the first, so it will appear above the first one and the same with the third, forth...

||INSERTAR IMATGE FUNCIONAMENT DE SPRITES||

### How we can control this sprite ordering? 

There is a data structure called Priority Queue in the STL library that has the particularity of reordering the elements that are stored inside with a predefined condition.   
