CFLAGS = -Wall -std=c++11 -I/usr/local/include -L/usr/local/lib -L/usr/local/lib64 -lglfw3 -lGLEW -lopenal -lGL -lalure2 -lIrrKlang -lXrandr -lXinerama -lXcursor -lXi -lXxf86vm -lX11 -pthread -lrt -lm -ldl -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib

a.out: main.o Play.o Result.o Title.o
	g++ main.o Play.o Result.o Title.o $(CFLAGS)

main.o: main.cpp shaderManager.h Window.h Figure.h DrawFigure.h TextureManager.h
	g++ -Wall -std=c++14 -c main.cpp

Play.o: Play.cpp State.h Play.h
	g++ -Wall -std=c++14 -c Play.cpp

Result.o: Result.cpp State.h Result.h
	g++ -Wall -std=c++14 -c Result.cpp

Title.o: Title.cpp State.h Title.h
	g++ -Wall -std=c++14 -c Title.cpp

clean:
	$(RM) *.o