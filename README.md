this is an implementation of a int generator using the new c++ 20's feature coroutines. It was written as an excerise for educational purposes, to expierement a bit with the new feature

# Usage
an example of a usage of the generator class is:
~~~
  generator gen;
  for(auto value : gen){
    if(value > 100)
      break;
    std::cout << value << std::endl;
  }
  ~~~
  this example prints all the numbers from 0 to 100
