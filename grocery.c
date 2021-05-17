//Mujtaba Mujtaba
//101167348

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BAGITEMS	25
#define CARTITEMS	100
#define MAXWEIGHT	5
#define ITEMSINCART 50

typedef struct{
	char *name;
	float cost;
	float weight;
	float perishable;
}GroceryItem;

typedef struct{
	GroceryItem *bagItems[BAGITEMS];
	int numItemsInBag;
	float totalWeight;
}Bag;

typedef struct{
	GroceryItem *cartItems[CARTITEMS];
	int itemsInCart;
	Bag **bagArr;
	int numOfBags;
}Cart;
//Prototypes.
int addToCart(GroceryItem *, Cart *);
int addToBag(GroceryItem *, Bag *);
void displayItem(GroceryItem *);
void displayItemsInBag(Bag *, int);
void displayItemsInCart(Cart *);
void pack(Cart *);
int removeFromBag(GroceryItem *, Bag *);
void removePerishables(Cart *);

//main function, used to initilaze grocery items
//it inililiazes them using a for loop
//then a struct of cart is created to represent a shopping cart which holds all the elements 
//50 elements are chosen with rand.
//after that all the items in cart are displayed.
//then after packing all the elements packed in bags and unpacked (in carts) are printed.
//then all the perishable items are removed with help of remove from bag.
//then again its printed.
//at the dynamically allocated memory for arrays is freed.
//with zero errors on valgrind.
int main(){
	srand(time(NULL));
	//the array used to initalize items w/ their respective properties
	GroceryItem testItems[12];
	char *sampleItemNames[] = {"Smart-Ones Frozen Entrees", "SnackPack Pudding", "Breyers Chocolate Icecream", "Nabob Coffee", "Gold Seal Salmon", "Ocean Spray Cranberry Cocktail", "Heinz Beans Original", "Lean Ground Beef", "5-Alive Frozen Juice", "Coca-Cola 12-pack", "Toilet Paper - 48 pack", "Crate of milk"};
	float sampleItemPrices[] = {1.99, 0.99, 2.99, 3.99, 1.99, 2.99, 0.79, 4.94, 0.75, 3.49, 40.96, 12.99};
	float sampleItemWeights[] = {0.311, 0.396, 2.27, 0.326, 0.213, 2.26, 0.477, 0.75, 0.426, 5.112, 10.89f, 6.18f};
	float sampleItemPerish[] = {1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1};
	//to fill in the grocery items.
	for (int y = 0; y < 12; y++){
		testItems[y].name = sampleItemNames[y];
		testItems[y].cost = sampleItemPrices[y];
		testItems[y].weight = sampleItemWeights[y];
		testItems[y].perishable = sampleItemPerish[y];
	}
	/*for (int i = 0; i<12; i++){
		//printf(" %s , %.3f , %.3f, %.3f\n", testItems[i].name, testItems[i].cost, testItems[i].weight, testItems[i].perishable);
		displayItem(&testItems[i]);
	}*/
	
	

	Cart shoppingCart;
	shoppingCart.itemsInCart = 0;
	
	
	for(int x = 0; x < 50; x++){
		int p = (int) (rand() % 12);
		addToCart(&testItems[p], &shoppingCart);
	}
	/*for (int i = 0; i<50; i++){
		displayItem(shoppingCart.cartItems[i]);
		printf("%d \n", shoppingCart.itemsInCart);
		//shoppingCart.itemsInCart--;
		//printf("%d \n", shoppingCart.itemsInCart);
		
	}*/

	shoppingCart.numOfBags = 0;
	printf("\n\nBEFORE PACKING ITEMS IN CART\n\n\n");
	displayItemsInCart(&shoppingCart);
	pack(&shoppingCart);
	
	printf("\n\nAFTER PACKING ITEMS INTO BAGS AND THEN PLACING THEM IN BAGS IN CART\n\n\n");
	displayItemsInCart(&shoppingCart);

	removePerishables(&shoppingCart);
	printf("\n\nAFTER REMOVING PERISHABLES FROM CART\n\n\n");
	displayItemsInCart(&shoppingCart);
	
	//This is used to free up memory from each bag array.
	for(int y = 0; y < shoppingCart.numOfBags ; y++){
		free(shoppingCart.bagArr[y]);	
	}
	//This is used to free up the array of Bags array.
	free(shoppingCart.bagArr);
	

	
	
	return 1;
		

}

//basic function to add items in cart.
//it first checks if the items in cart cross the carts limit (100)
//if it does then it returns 0
//if it doesnt it adds the item to cart using the integer items in cart for index.
//then it returns 1
int addToCart(GroceryItem *item, Cart *shoppingCart){
	int x = 0;
	if(shoppingCart->itemsInCart > CARTITEMS){
		return 0;
	}
	x = shoppingCart->itemsInCart;
	shoppingCart->cartItems[x] = item;
	shoppingCart->itemsInCart++;
	return 1;
	 
}

//basic function to add items in bag.
//it first checks if the items in bag cross the bags limit (25)
//if it does then it returns 0
//if it doesnt it adds the item to bag using the integer items in bag for index.
//then it returns 1
//similar to addto cart
int addToBag(GroceryItem *item, Bag *bg){
	if(bg->numItemsInBag > BAGITEMS){
		return 0;
	}
	bg->bagItems[bg->numItemsInBag] = item;
	bg->numItemsInBag++;
	bg->totalWeight += item->weight;
	return 1;
	
}



//Basic print function which prints out "item" passed in by paramter.
//All single items are printed in this specific format.
void displayItem(GroceryItem *item){
	printf("%s weighing %.3fkg, with price $%.2f \n", item->name, item->weight, item->cost);
}

//Prints the total weight of THAT bag, as well as the bag number since arrays start from 0 (+1) is needed in print.
//explained in steps inside the function
void displayItemsInBag(Bag *bag, int bagNumber){
	//it takes in pointer to grocery item, and integer representing bag number.
	//should show bag number and total weight of bag.
	//then display all items in bag	

	printf("\nBag %d (Total Weight %.3fkg ) \n\n",bagNumber+1, bag->totalWeight);
	int indexOfHeaviestItem = 0;
	//This goes over the array of bag to check the heaviest item
	//and then it stores the index of the heaviest item
	//which is then used in the next for loop
	for (int x = 0; x < bag->numItemsInBag; x++){
		if(bag->bagItems[x]->weight > bag->bagItems[indexOfHeaviestItem]->weight){
			indexOfHeaviestItem = x;
		}

	}
	//To iterate over the elements in the array of bag.
	for (int p = 0; p < bag->numItemsInBag; p++){
	// " * " indented next to heaviest item
	//if there is more than one heavy item print * beside it as well
		if(bag->bagItems[indexOfHeaviestItem]->weight == bag->bagItems[p]->weight){
			printf(" *");
			displayItem(bag->bagItems[p]);
			continue;
		}
		// must make use of displayItem.
		displayItem(bag->bagItems[p]);
	}
	
	
	
	

}

//makes use of the 2 display functions above
//explained in detail inside the function
void displayItemsInCart(Cart *shoppingCart){
	//Shows unpacked items that is the items whose weight is over 5 kg
	for (int x = 0; x < shoppingCart->itemsInCart; x++){
		displayItem(shoppingCart->cartItems[x]);
	}	
	
	
	//followed by packed items
	//this is to iterate over the array of bags array, using the number of arrays which is an int
	//then uses the function above for each bag.
	for(int i = 0; i < shoppingCart->numOfBags ; i++){
		displayItemsInBag(shoppingCart->bagArr[i] , i);
	}	
	
}


//After packing the cart, cart should have loose items and packed bags.
//The array of bags in the cart must be dynamically allocated to the exact number of bags needed.
	//either create a bigger array of bags.
	//first check how many bags you need.
//Put the item from cart in their sequential order and put them into a bag until the next item would cause the
//bag to exceed its weight or items limit.
//If that happens make new bag.	
//Once all is done no unpacked item should be in cart unless its weight exceeds 5 kg
//explained in more details inside the function.
void pack(Cart *shoppingCart){
	//Dynamically allocate an array of bags array and store it in cart.
		//then fill up the bag until full
	Bag **bagArr;
	int bagsNeeded = 1;
	float weightOfItems = 0;
	int itemsInBag = 0;
	
	

	//to determine the number of bags required, similar steps can be used for actually adding elements.
	for (int x = 0; x < shoppingCart->itemsInCart; x++){
		//check if the items weight is over 5 kg
		if(shoppingCart->cartItems[x]->weight > MAXWEIGHT){
			//printf("%s , weight = %.3f, sorry the item's weight exceeded 5 kgs \n",shoppingCart->cartItems[x]->name ,shoppingCart->cartItems[x]->weight);
			continue;
		}
		//check if the weight of bag crosses 5 if the item is added, if it does make new bag.
		//or if the items in a bag cross the 25 limit. 
		//use addToBag function in both cases.
		//but in else you dont need a new bag.
		if(weightOfItems + shoppingCart->cartItems[x]->weight > MAXWEIGHT || itemsInBag > BAGITEMS){
			//printf("WEIGHT EXCEEDED 5kg NEW BAG-----------------------------------------------\n");
			
			bagsNeeded++;
			itemsInBag = 0;
			weightOfItems = 0;
			itemsInBag++;
			weightOfItems += shoppingCart->cartItems[x]->weight; 
			//printf("weight of bag = %.2f \n", weightOfItems);
			//printf("items in bag = %d \n", itemsInBag);
			//printf("%s , weight = %.3f\n",shoppingCart->cartItems[x]->name ,shoppingCart->cartItems[x]->weight);

			
		}else{
			//printf("%s , weight = %.3f\n",shoppingCart->cartItems[x]->name ,shoppingCart->cartItems[x]->weight);
			//printf("hello from else \n");
			weightOfItems += shoppingCart->cartItems[x]->weight;
			itemsInBag++;
			
		}		
	}
	//printf("%.2f\n", weightOfItems);
	//printf("%d\n", itemsInBag);
	//printf("%d\n", bagsNeeded);
	shoppingCart->numOfBags = bagsNeeded;
	//allocates memory for array of bags arrays
	bagArr = (Bag **) malloc (bagsNeeded * sizeof(Bag));
	if(bagArr == NULL){
		printf("Failure to allocate memory, buy a pc with more ram.\n");
		exit(-1);
	}
	//allocates memory for seperate each bag array.
	//also initializes the value to avoid errors in valgrind
	for (int b = 0; b < bagsNeeded; b++){
		bagArr[b] = NULL;
		bagArr[b] = (Bag *) malloc (sizeof(Bag));
		if(bagArr[b] == NULL){
			printf("Failure to allocate memory, buy a pc with more ram.\n");
			exit(-1);
		}

		bagArr[b]->numItemsInBag = 0;
		bagArr[b]->totalWeight = 0;
		
	}
	//some variables to keep track of items 	
	float weightInBag = 0;
	int numItemsInBag = 0;
	int bagCounter = 0;

	for(int y = 0; y < shoppingCart->itemsInCart; y++){
		//check if the items weight is over 5 kg
		if(shoppingCart->cartItems[y]->weight > MAXWEIGHT){
			continue;
		}
		//check if the weight of bag crosses 5 if the item is added, if it does make new bag.
		//or if the items in a bag cross the 25 limit. 
		//use addToBag function in both cases.
		//but in else you dont need a new bag.
		if(weightInBag + shoppingCart->cartItems[y]->weight > MAXWEIGHT || numItemsInBag > BAGITEMS){
			bagCounter++;
			numItemsInBag = 0;
			weightInBag = 0;
			weightInBag += shoppingCart->cartItems[y]->weight;
			numItemsInBag++; 
			addToBag(shoppingCart->cartItems[y], bagArr[bagCounter]);
	
		}else{
			weightInBag += shoppingCart->cartItems[y]->weight;
			numItemsInBag++;
			addToBag(shoppingCart->cartItems[y], bagArr[bagCounter]);
		}
		
		GroceryItem *c;	
		//to rearrange items (similar approach used in removeFromBags.	
		for(int p = y; p < shoppingCart->itemsInCart-1; p++){
			c = shoppingCart->cartItems[p+1];
			shoppingCart->cartItems[p] = c;
		}
		//when the item is moved over from cart to bag the unpacked items in cart decrease.
		//so to accomodate that decrease you need to stay on same index by using y--, to balance the y++
		//otherwise you'll skip out on elements.
		shoppingCart->itemsInCart--;
		y--;
			

	}
	//to store it in the cart...
	shoppingCart->bagArr = bagArr;
	return;
	/*for (int b = 0; b < bagsNeeded; b++){
		for(int i = 0; i < shoppingCart->bag->numItemsInBag; i++){
			printf("%s , weight = %.2f\n",bagArr[b]->bagItems[i]->name ,bagArr[b]->bagItems[i]->weight);
		}
		printf("%d , %.3f\n", bagArr[b]->numItemsInBag, bagArr[b]->totalWeight); 
	}*/
}

//this function is used to remove perishables from each bag
//it searches bag by bag
//first checks if bag is empty, if it is, it returns a 0 (nothing can be removed).
//then it searches the bag for that item and if that is found is found
//it removes it by moving the element in the next index to it's index.
//after doing that the items in bags are decreased
//so is the weight. and then return 1
//if item not found return 0

int removeFromBag(GroceryItem *item, Bag *bg){
	if(bg->numItemsInBag == 0){
		return 0;
	}
	GroceryItem *y;
	for(int x = 0; x < bg->numItemsInBag; x++){
		if(bg->bagItems[x] == item){
			for(int p = x; p < bg->numItemsInBag-1; p++){
				y = bg->bagItems[p+1];
				bg->bagItems[p] = y;
			}
			bg->numItemsInBag--;
			bg->totalWeight -= item->weight;
			return 1;
		}
	}
	return 0;				 		
	
}
//first checks the unpacked items in cart, (milk) removes that from cart
//then reduces the items in cart, also resets the index by 1 (--)
//then uses similar approach for bags
//however there is a function to remove items from bag
//but resets the index by 1 (--).
void removePerishables(Cart *shoppingCart){
	GroceryItem *y;

	//printf("Items in cart before removing %d \n", shoppingCart->itemsInCart);
	for (int x = 0; x < shoppingCart->itemsInCart; x++){		
		if(shoppingCart->cartItems[x]->perishable == 1){
			//removeFromCart();
			for(int p = x; p < shoppingCart->itemsInCart-1; p++){
				y = shoppingCart->cartItems[p+1];
				shoppingCart->cartItems[p] = y;
			}	
			shoppingCart->itemsInCart--;
			x--;
		}
	}
	//printf("Items in cart after removing %d \n", shoppingCart->itemsInCart);
	
	for(int y = 0; y < shoppingCart->numOfBags ; y++){
		for( int k = 0; k < shoppingCart->bagArr[y]->numItemsInBag ; k++){
			if (shoppingCart->bagArr[y]->bagItems[k]->perishable == 1){
				removeFromBag(shoppingCart->bagArr[y]->bagItems[k], shoppingCart->bagArr[y]);
				k--;
			}	
		}
	}
}



