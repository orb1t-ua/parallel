Building main program:
	make parallel

Building data export program:
	rm *.o *.out
	make export
	
Building OpenGL visualization program:
	You must have OpenGL headers, GLEW, glfw3 and ability to create OpenGL3.3 core context
	cd animation
	./build.sh
	
Running main program or export program:
	./p4.out <Threads> <Width> <Height> <Generations>
	
Running visualization program:
	cd animation
	./visualizer <Width> <Height> <Threads>
	
	The visualization program doesn't really get any benefit from additional threads.
	One thread is already too fast for how big a screen can get, and vertical sync is on.
