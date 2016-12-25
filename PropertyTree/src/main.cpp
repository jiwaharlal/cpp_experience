#include <iostream>
#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char** argv)
{
   std::cout << "hello" << std::endl;

   std::string root;
   if (argc == 2)
   {
      root = argv[1];
   }

   QApplication a(argc, argv);
   MainWindow w(root.c_str());
   w.resize(1000, 600);
   w.show();

   return a.exec();
}
