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
