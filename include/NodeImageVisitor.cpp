
#include <osg/io_utils>

#include <osgDB/WriteFile>

#include "NodeImageVisitor.h"


void NodeImageVisitor::setSavePath( std::string savePaths )
{
    savePath = savePaths;
}

NodeImageVisitor::ImageMaterial::ImageMaterial(
    osg::Material* mat ,
    osg::Texture* tex ,
    std::string save_string ,
    bool outputTextureFiles ,
    const osgDB::Options* options ) :
    image( "" )
{
    if (tex)
    {
        osg::Image* img = tex->getImage( 0 );

        if ((img) && (!img->getFileName().empty()))
        {
            if (save_string.empty())
                image = osgDB::getSimpleFileName( img->getFileName() );
            else
                image = save_string + "\\" + osgDB::getSimpleFileName( img->getFileName() );

            if (outputTextureFiles)
            {
                std::string imagePath = osgDB::getFilePath( image );
                if (!imagePath.empty() && !osgDB::fileExists( imagePath ))
                {
                    osgDB::makeDirectory( imagePath );
                }
                osgDB::writeImageFile( *img , image , options );
            }
        }
    }

}


void NodeImageVisitor::processStateSet( osg::StateSet* ss )
{
    if (_materialMap.find( ss ) != _materialMap.end())
    {
        return;
    }

    osg::Material* mat = dynamic_cast<osg::Material*>(ss->getAttribute( osg::StateAttribute::MATERIAL ));
    osg::Texture* tex = dynamic_cast<osg::Texture*>(ss->getTextureAttribute( 0 , osg::StateAttribute::TEXTURE ));

    if (mat || tex)
    {
        _materialMap.insert( std::make_pair( osg::ref_ptr<osg::StateSet>( ss ) , ImageMaterial( mat , tex , savePath , _outputTextureFiles , _options.get() ) ) );
    }

}

void NodeImageVisitor::processGeometry( osg::Geometry* geo , osg::Matrix& m )
{
    if (geo->containsDeprecatedData())
        geo->fixDeprecatedData();

    processStateSet( _currentStateSet.get() );
}

void NodeImageVisitor::apply( osg::Geometry& geometry )
{
    osg::Matrix m = osg::computeLocalToWorld( getNodePath() );

    pushStateSet( geometry.getStateSet() );

    processGeometry( &geometry , m );

    popStateSet( geometry.getStateSet() );
}

void NodeImageVisitor::apply( osg::Geode& node )
{
    pushStateSet( node.getStateSet() );
    // _nameStack.push_back( node.getName() );
    unsigned int count = node.getNumDrawables();
    for (unsigned int i = 0; i < count; i++)
    {
        node.getDrawable( i )->accept( *this );
    }
    popStateSet( node.getStateSet() );
    // _nameStack.pop_back();
}



