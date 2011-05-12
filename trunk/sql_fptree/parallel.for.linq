<Query Kind="Program">
  <Namespace>System.Threading.Tasks</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
	Parallel.For(11, 17+1, i => {Console.Write(i);});
}

// Define other methods and classes here
