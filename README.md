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

<p align="center"> 
<img src="https://github.com/MaxitoSama/Sprite-ordering-and-Camera-Culling/blob/master/Assigment_1/Game/wiki/Sin%20t%C3%ADtulo-1.png" width="800">
</p>

### How we can control this sprite ordering? 

There is a data structure called Priority Queue in the STL library that has the particularity of reordering the elements that are stored inside with a predefined condition. 

However, which is this condition? We can imagine our 2D game as a 3D game where the Z axis will be the order of the sprites. So now our sprites will need a new variable. We can say that sprites with smaller numbers will be printed befor sprites with bigger numbers, simple as that condition!

Now it's time to implement that in C++. The first thing that you hav to do is create a new class called "ObjectToPrint". The variables of this class will be the same as the parameters of your render funtion (position x and y, scale, rectangle...) and the new parameter Z or priority.

Now that we have our new class it's time to create our priority queue. The STL priority queue needs 3 template parameter:

```
std::priority_queue <class T, class Container = vector<T>, class Compare = less<typename Container::value_type> > name;
```

* **T:** Define the type of elements of the queue.
* **Vector Container:** Type of the internal container where the elements are stored.
* **Compare**: A binary predicate that takes two elements from the vector, so they are type T, and then they are compared and returns a bool. In order to compare the elements and know the order is used the function comp(a,b) where comp is an object of type T and a and b are two elements of the vector container. The bool considers if a has to go before b or not.

You will have to define the function that compares the elements and the best way is to create an struct with a boolean operator that needs the type of elements that you want to compare, in this case the struct will be:

```
struct OrderPriority
{
    bool operator()(const ObjectToPrint* Object_1, const ObjectToPrint* Object_2)
    {
    //priority condition
    }
}
```
In the end, our priority queue will has this declaration:

```
std::priority_queue <ObjectToPrint*,vector<ObjectToPrint*>,OrderPriority> SpriteOrdererQueue;
```

Now that you have defined the priority queue we have to create a method that fills it and then, when all the frame logic been done, prints the sprites in the correct order. This can be done in two steps:
* **void FillQueue():** This function has to replace your render function, because now we are going to send the sprites to the queue before the rendering. This function has to have the same arguments as your render funtion. It has to create a new ObjectToPrint elements and push it to the queue using SpriteOrdererQueue.push(ObjectToPrint).

* **Render the Queue:** Once your priority queue has all your sprites in the correct order, it's time to render the sprites. You have to modify your Render function. Your render function now will recieve your priority queue as a parameter and it will have to render your sprites one by one while your queue is not empty. Remember to make pop of each element that has been printed. 

### Improvement:

Notice that you can change the parameters of the Z priority in any time, so you can achieve things like this one:

<p align="center"> 
<img src="https://github.com/MaxitoSama/Sprite-ordering-and-Camera-Culling/blob/master/Assigment_1/Game/wiki/Sprite%20Order.gif" width="400">
</p>


## Camera Culling

Camera Culling is a method used to optimize our games and the idea it's easy to understand. Now, your games is constantly rendering all the entities and the whole map, even if you are seeing it or not. The point here is that we don't want to render all these parts of the map and all these entities that are not being seen through the camera. This is the Camera Culling, a filter in the render funtion that says if an object can be rendered or not.

<p align="center"> 
<img src="https://github.com/MaxitoSama/Sprite-ordering-and-Camera-Culling/blob/master/Assigment_1/Game/wiki/Camera%20Culling.png" width="600">
</p>

### How can you do an easy Camera Culling effect?

Well, as I've said, you have to create a filter in the render function that desides if an entity or a tile of the map has to be rendered or not. This filter will be a collision detector. You have to create a funtion that detects if an object is colliding with the camera or not. This function must return a bool where true means render and false means not render.

However, you have created a priority queue where you have all your ObjectsToPrint and you also have a function that fills this priority queue. It will be faster if you first check which objects collide with the camera and then, if the collision is true, put them inside the priority queue where they will be ordererd than put all the objects inside the priority queue, order them and then check the collisions. Why? Because we are goig to check the collision with the camera with all the objects and if we first check the collision then we will order a few number of them intead of order all the objects and check all the collisions.

You only have to modify the **FillQueue()** function created befor:

```
void j1Render::FillQueue(...)
{
	if (CameraCollision(ObjectToPrint->Rect))
	{
		ObjectToPrint* auxObject = new ObjectToPrint(...);
		SpriteOrderer.push(ObjectToPrint);
	}
}
```

Where:

```
bool j1Render::CameraCollision(const SDL_Rect& rect)const
{
	if ((rect.x < -camera.x + camera.w && rect.x + rect.w > -camera.x) || (rect.x < -camera.x + camera.w  && rect.x + rect.w > -camera.x))
	{
		if (rect.y < -camera.y + camera.h && rect.y + rect.h > -camera.y)
		{
			return true;
		}
	}
	return false;
}

```

### First Results

Here you have two gifs that show how works this camera culling. On the left is the vision of the camera and what the player will see. On the right you can see how works the camera culling.

<p align="center"> 
<img src="https://github.com/MaxitoSama/Sprite-ordering-and-Camera-Culling/blob/master/Assigment_1/Game/wiki/Camera%20Culling_1.gif" width="400">
<img src="https://github.com/MaxitoSama/Sprite-ordering-and-Camera-Culling/blob/master/Assigment_1/Game/wiki/Camera%20Culling%202.gif" width="325">
</p>

## Quadtrees

Checking a big number of collisions every frame is a big waste of resources for our game. Every frame, our computer is checking if one entity is colliding with all the rest of entities and the same for each entity. For example, if you have 200 entities and you want to know how many entities your games is going to check every frame the solution is 200x200=40.000 operation. It's a really big number of operations.

Here is the question: Do I have to calculate the collision between two objects that are very fare one from each other? The answer is No (If you are interested in optimize your game).

### Space Partitioning

The first solution is dividing our map in small cells or rectangles and then calculate only the collisions of the entities that are in the same rectangle. This is known as Space partitioning. However, there is a probleme with that method. If you have a lot of entities in the same rect or buch of rects then you will have to calculate the collision between big number of entities again. 

<p align="center"> 
<img src="https://github.com/MaxitoSama/Sprite-ordering-and-Camera-Culling/blob/master/Assigment_1/Game/wiki/spatial%20partitioning_1.png" width="200">
<img src="https://github.com/MaxitoSama/Sprite-ordering-and-Camera-Culling/blob/master/Assigment_1/Game/wiki/spatial%20partitioning_2.png" width="200">
</p>

In order to solve that problem, you will have to create the Quadtree class.

### Quadtrees

A Quadtree is an other type of space partitioning, but instead of having an static grid of nodes or cells where the game calculates the collisions of all the entities that are inside, it generates automatically its own partitions. It's a tree data structure, that are one of the fastest data structures. Now I'm going to make you an acurate description of how it works.

* First you have a number of entities in your map, lets say 3. This number will be the maximum number of entities that can be inside a rect.

||IMATGE UN CUADRAT 3 ENTITATS|| IMATGE DE L'ARBRE|

* If you insert another entity in the map, then the quadtree starts its work. It will divide your initial rect in four smaller rects. Now, the game will only calculate the collisions of the entities that are in the same new rects. 

||IMATGE 4 CUADRATS 4 ENTITATS|| IMATGE DE L'ARBRE|

* If you add more and more entities, the quadtree will be dividing more and more each rect in 4 smaller rects.

||IMATGE x CUADRATS x ENTITATS|| IMATGE DE L'ARBRE|

As you can see, we are generating smaller regions in order to reduce the amount of collisions checkings. This is a video that shows how it works in real time.

[![Quadtree video](https://github.com/MaxitoSama/Sprite-ordering-and-Camera-Culling/blob/master/Assigment_1/Game/wiki/maxresdefault.jpg )](https://www.youtube.com/embed/TJzq_kjtGTc)


### Quadtree in C++

As I've said before, a quadtree is a tree structure wich means that you star in a node and from that node you go to its childrens. The faster way to do it is using recursive functions. The class template will be something like this one:

```
class Quadtree
{
public:
	Quadtree(SDL_Rect rect)
	{
		Space = rect;
		for (int i = 0; i < Children.size(); i++)
		{
			Children[i] = nullptr;
		}
	}

	~Quadtree(){ Clear();}

	void Clear();
	void Split();
	bool insert();
	bool CheckBoundaries();
	list<Collider*> FillCollisionList();

public:
	
	SDL_Rect		Space; 		//Rect that deffine the node space
	list<Collider*>		Objects;	//a list to store all the entities
	array<Quadtree*,4>	Children;	//an empty array for the childrens

};
```

You will need the following functions (You can find it in the code source):

* **Clear():** This function goes node by node deleting all the Entities stored inside each one. 
* **Split():**	This function divides the parent node into 4 children nodes when the current node reach the machimum number of entities.
* **Insert():** This function insert all the entities in their apropriate child and if its necessary calls the split() function.
* **ColliderList():** This function recieves an entity and it returns a list with all the entities that the collider maneger will have to check.
* **CheckBoundaries()** Check if two rects are colliding.

## Using The Quadtree in Camera Culling

If we want to implement our Quadtree class in a camera cullin method, what we have to do is:
* Initialize the Quadtree with the map size rect.
* Insert all the entities inside the Quadtree using the Inset() function.
* Now that you have all the entities organized iside the quadtree, you can call the ColliderList(camera_rect) function with the camera rectangle. It will check where is the camera and with which rectangles it is colliding. Then the function will return a list of entities that are inside these rectangles. We hav to check if an entity can be or not inside the camera. If they are inside the camera we will send it to the Priority queue and the we will render it!

---

# Links:
[Easy way to create your Quadtree class and implementate it](https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374)

[Efficient and Well explainde Qadtree Collision](https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det)

[What is a Quadtree? (spanish version)](https://www.genbetadev.com/programacion-de-videojuegos/teoria-de-colisiones-2d-quadtree)

[Camera Culling idea](https://www.youtube.com/watch?v=zCaurIC49I4)

[spatial Partition tutorial](https://www.youtube.com/watch?v=RN1GRX2ByLM&t=1950s)
