// 用于访问节点里的纹理并写出到磁盘

#ifndef OBJ_WRITER_NODE_VISITOR_HEADER__
#define OBJ_WRITER_NODE_VISITOR_HEADER__


#include <string>
#include <stack>
#include <map>
#include <set>

#include <osg/Node>
#include <osg/Geode>

#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexMat>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/OutputStream>

class NodeImageVisitor : public osg::NodeVisitor
{

    public:
    NodeImageVisitor( bool outputTextureFiles = false , const osgDB::Options* options = NULL ) :
        osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ) ,
        _currentStateSet( new osg::StateSet() ) ,
        _outputTextureFiles( outputTextureFiles ) ,
        _options( options )
    {

    }

    virtual void apply( osg::Geometry& geometry );

    virtual void apply( osg::Geode& node );

    virtual void apply( osg::Group& node )
    {
        pushStateSet( node.getStateSet() );
        _nameStack.push_back( node.getName().empty() ? node.className() : node.getName() );

        traverse( node );

        _nameStack.pop_back();

        popStateSet( node.getStateSet() );
    }

    void pushStateSet( osg::StateSet* ss )
    {
        if (NULL != ss)
        {
            // Save our current stateset
            _stateSetStack.push( _currentStateSet.get() );

            // merge with node stateset
            _currentStateSet = static_cast<osg::StateSet*>(_currentStateSet->clone( osg::CopyOp::SHALLOW_COPY ));
            _currentStateSet->merge( *ss );
        }
    }


    void popStateSet( osg::StateSet* ss )
    {
        if (NULL != ss)
        {
            // restore the previous stateset
            _currentStateSet = _stateSetStack.top();
            _stateSetStack.pop();
        }
    }

    void setSavePath( std::string ); // set output image folder path

    class ImageMaterial
    {
        public:
        ImageMaterial() {}
        ImageMaterial( osg::Material* mat , osg::Texture* tex , std::string save_string , bool outputTextureFiles = false , const osgDB::Options* options = NULL );
        std::string    image;
        std::string name;
    };

    protected:
    struct CompareStateSet
    {
        bool operator()( const osg::ref_ptr<osg::StateSet>& ss1 , const osg::ref_ptr<osg::StateSet>& ss2 ) const
        {
            return ss1->compare( *ss2 , true ) < 0;
        }
    };


    private:

    NodeImageVisitor& operator = ( const NodeImageVisitor& ) { return *this; }

    void processGeometry( osg::Geometry* geo , osg::Matrix& m );

    void processStateSet( osg::StateSet* stateset );

    typedef std::stack<osg::ref_ptr<osg::StateSet> > StateSetStack;
    typedef std::map< osg::ref_ptr<osg::StateSet> , ImageMaterial , CompareStateSet> MaterialMap;


    std::list<std::string>                  _nameStack;
    StateSetStack                           _stateSetStack;
    osg::ref_ptr<osg::StateSet>             _currentStateSet;
    std::map<std::string , unsigned int>     _nameMap;
    MaterialMap                             _materialMap;
    bool                                    _outputTextureFiles;
    osg::ref_ptr<const osgDB::Options>      _options;
    std::string savePath;  // output image folder path 
};

#endif
