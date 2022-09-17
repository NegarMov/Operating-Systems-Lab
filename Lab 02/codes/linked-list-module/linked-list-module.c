#include <linux/init.h> 
#include <linux/kernel.h>
#include <linux/module.h> 
#include <linux/list.h> 
#include <linux/slab.h> 

static LIST_HEAD(birthday_list); 

struct birthday { 
	int day;  
	int month;  
	int year;  
	struct list_head list; 
};

/* this function is called when the module is loaded */ 
int simple_init(void) {  
	printk(KERN_INFO "Loading Birthday Module...\n");  

	struct birthday *person1; 
	person1 = kmalloc(sizeof(person1), GFP_KERNEL); 
	person1->day = 12; 
	person1->month = 10; 
	person1->year = 2001; 
	INIT_LIST_HEAD(&person1->list);
	list_add_tail(&person1->list, &birthday_list);

	struct birthday *person2; 
	person2 = kmalloc(sizeof(person2), GFP_KERNEL); 
	person2->day = 16; 
	person2->month = 5; 
	person2->year = 2002; 
	INIT_LIST_HEAD(&person2->list);
	list_add_tail(&person2->list, &birthday_list);

	struct birthday *person3; 
	person3 = kmalloc(sizeof(person3), GFP_KERNEL); 
	person3->day = 5; 
	person3->month = 9; 
	person3->year = 1997; 
	INIT_LIST_HEAD(&person3->list);
	list_add_tail(&person3->list, &birthday_list);

	struct birthday *person4; 
	person4 = kmalloc(sizeof(person4), GFP_KERNEL); 
	person4->day = 23; 
	person4->month = 5; 
	person4->year = 1980; 
	INIT_LIST_HEAD(&person4->list);
	list_add_tail(&person4->list, &birthday_list);

	struct birthday *person5; 
	person5 = kmalloc(sizeof(person5), GFP_KERNEL); 
	person5->day = 16; 
	person5->month = 4; 
	person5->year = 2005; 
	INIT_LIST_HEAD(&person5->list);
	list_add_tail(&person5->list, &birthday_list);

	struct birthday *ptr;
	int i = 1;
	list_for_each_entry(ptr, &birthday_list, list)
		printk(KERN_INFO "%d.  %d/%d/%d\n", i++, ptr->year, ptr->month, ptr->day);

	printk(KERN_INFO "End of init function. Added all nodes successfully!\n");

	return 0; 
} 

/* this function is called when the module is removed */
void simple_exit(void) {  
	printk(KERN_INFO "Removing Birthday Module...\n"); 

	int i = 1;
	struct list_head *ptr, *tmp;
	list_for_each_safe(ptr, tmp, &birthday_list){
		printk(KERN_INFO "Freeing node %d\n", i++); 
		list_del(ptr);
		kfree(ptr);
	}

	printk(KERN_INFO "End of exit function. removed all nodes successfully!\n");
} 

/* Macros for registering module entry and exit points */ 
module_init(simple_init); 
module_exit(simple_exit); 
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("A module written to work with Linux kernel data structures"); 
MODULE_AUTHOR("Negar Movaghatian - 9831062");
