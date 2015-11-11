CC = g++
CFLAGS = -L/usr/X11R6/lib -lX11 -lopencv_core -lopencv_imgproc -lopencv_objdetect -lopencv_highgui -lzbar
XLIB= -L/usr/X11R6/lib -lX11
pam_qrcode.so:
	${CC} -fPIC -DPIC -shared -rdynamic -o pam_qrcode.so PAMQRCodeAuth.cpp model/Challenge.cpp model/Response.cpp util/Parser.cpp ${XLIB} ${CFLAGS}

