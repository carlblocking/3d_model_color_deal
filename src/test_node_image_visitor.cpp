#include "NodeImageVisitor.h"

int main(){
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("C:/Users/86183/Desktop/2/Tile_+000_+003_L22_0000210.osgb");

    NodeImageVisitor nv(true, nullptr);
    nv.setSavePath("C:/Users/86183/Desktop/2");
    node->accept(nv);

    return 0;
}