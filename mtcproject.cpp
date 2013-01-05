/*MASTER TILE CONVERTER

Copyright (C) 2012  JM Muller

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mtcproject.h"
#include <iostream>
#include <QMessageBox>

MTCbookmark::MTCbookmark(unsigned long offset,short mode,QString description) :
    m_offset(offset),m_mode(mode),m_description(description)
{

}

MTCbookmark::MTCbookmark(QDomElement &e)
{
    bool ok;
    m_offset=e.attribute( "offset", "" ).toULong(&ok,16);
    m_mode=e.attribute( "mode", "" ).toULong(&ok);
    m_description=e.attribute( "description", "" );
}

QDomElement MTCbookmark::toNode(QDomDocument &d)
{
    QDomElement cn = d.createElement( "bookmark" );
    cn.setAttribute( "offset", QString("%1").arg(m_offset,0,16) );
    cn.setAttribute( "mode", m_mode );
    cn.setAttribute( "description", m_description );
    return cn;
}


//-------------------------------------------------------------

MTCproject::MTCproject(QString filename) : m_current_palette_index(0),
    m_current_mode(3),m_current_offset(0x10eb1),m_sprite_part_of_palette(true)
{
    std::cout<<"Create new MTCproject"<<std::endl;
    m_filename=filename;
    if (filename!="")
    {
        m_filename=filename;
        //todo: load file
    }else{
        m_palettes.push_back(new Palette("Empty")); //TODO ??
        m_filename="MTC.xml";
    }
}

MTCproject::~MTCproject()
{
    clear();

}

void MTCproject::clear()
{
    {
        std::vector<MTCbookmark*>::iterator bookmark_it;
        for( bookmark_it = m_bookmarks.begin(); bookmark_it != m_bookmarks.end(); ++bookmark_it)
        {
            delete (*bookmark_it);
        }
        m_bookmarks.clear();
        }
    {
        std::vector<Palette*>::iterator bookmark_it;
        for( bookmark_it = m_palettes.begin(); bookmark_it != m_palettes.end(); ++bookmark_it)
        {
            delete (*bookmark_it);
        }
        m_palettes.clear();
    }
}

bool MTCproject::load_ROM(QString ROMpath)
{
    if (!m_rom->loadfile(ROMpath.toStdString()))
    {
        QMessageBox::information(NULL, "Error.", QString("Error reading ")+ROMpath);
        return false;
    }
    m_ROM_filename=ROMpath;
    return true;
}

Palette* MTCproject::setPalette(int i)
{
    if (i<m_palettes.size())
        m_current_palette_index=i;
    return m_palettes.at(m_current_palette_index);
}

Palette* MTCproject::addPalette(QString description,long offset)
{
    Palette* newpal=new Palette(description);
    newpal->read_from_romdata(m_rom->get_romdata(),offset);
    m_palettes.push_back(newpal);
    m_current_palette_index=m_palettes.size()-1;
    return m_palettes.at(m_current_palette_index);
}

Palette* MTCproject::addPalette(QString description,QString filename)
{
    Palette* newpal=new Palette(description);
    newpal->read_from_file(filename);
    m_palettes.push_back(newpal);
    m_current_palette_index=m_palettes.size()-1;
    return m_palettes.at(m_current_palette_index);
}


void MTCproject::add_bookmark(QString description, unsigned long offset, short mode)
{
    MTCbookmark* bookmark=new MTCbookmark(offset,mode,description);
    m_bookmarks.push_back(bookmark);
}

void MTCproject::add_bookmark(MTCbookmark *b)
{
    m_bookmarks.push_back(b);
}


bool MTCproject::save_project(QString filename)
{
    QDomDocument doc( "MTCproject" );
    QDomElement root = doc.createElement( "MTCproject" );
    doc.appendChild( root );

    QDomElement ROM_node = doc.createElement( "ROM" );
    root.appendChild( ROM_node );
    ROM_node.setAttribute( "path", m_ROM_filename );

    QDomElement start_node = doc.createElement( "start" );
    root.appendChild( start_node );
    MTCbookmark start(m_current_offset,m_current_mode,"Start Position");
    start_node.appendChild( start.toNode(doc) );

    QDomElement bookmarks_node = doc.createElement( "bookmarks" );
    root.appendChild( bookmarks_node );
    std::vector<MTCbookmark*>::iterator bookmark_it;
    for( bookmark_it = m_bookmarks.begin(); bookmark_it != m_bookmarks.end(); ++bookmark_it)
    {
        bookmarks_node.appendChild((*bookmark_it)->toNode(doc));
    }

    QDomElement palettes_node = doc.createElement( "palettes" );
    root.appendChild( palettes_node );
    std::vector<Palette*>::iterator palette_it;
    for( palette_it = m_palettes.begin()+1; palette_it != m_palettes.end(); ++palette_it)//don't save default palette
    {
        palettes_node.appendChild((*palette_it)->toNode(doc));
    }


    QFile file( filename );
    if( !file.open( QIODevice::WriteOnly ) )
        return false;

    QTextStream ts( &file );
    ts << doc.toString();

    file.close();

    return true;
}


MTCbookmark * MTCproject::read_project(QString filename)
{
    clear();
    m_palettes.push_back(new Palette("Empty"));
    MTCbookmark *start=0;

    bool all_ok=true;

    QDomDocument doc( "MTCproject" );
    QFile file( filename );
    if( !file.open( QIODevice::ReadOnly ) ){
        std::cout<<"Unable to open xml file "<<filename.toStdString()<<std::endl;
        return 0;
    }else{
        if( !doc.setContent( &file ) )
        {
            file.close();
            std::cout<<"Unable to read xml"<<std::endl;
            return 0;
        }else{
            file.close();
            QDomElement root = doc.documentElement();
            if( root.tagName() != "MTCproject" ) {
                std::cout<<"Wrong root element"<<std::endl;
                return 0;
            }else{
                //read
                QDomNode n1 = root.firstChild();
                while( !n1.isNull() ) {
                    QDomElement e1 = n1.toElement();
                    if( !e1.isNull() ) {
                        if( e1.tagName() == "ROM" ) {
                            all_ok&=load_ROM(e1.attribute( "path", "" ));
                        }
                        if( e1.tagName() == "start" ) {
                            QDomNode n2 = n1.firstChild();
                            while( !n2.isNull() ) {
                                QDomElement e2 = n2.toElement();
                                if( !e2.isNull() ) {
                                    if( e2.tagName() == "bookmark" ) {
                                        start=new MTCbookmark(e2);
                                        m_current_offset=start->get_offset();
                                        m_current_mode=start->get_mode();
                                    }
                                }
                                n2 = n2.nextSibling();
                            }
                        }
                        if( e1.tagName() == "bookmarks" ) {
                            QDomNode n2 = n1.firstChild();
                            while( !n2.isNull() ) {
                                QDomElement e2 = n2.toElement();
                                if( !e2.isNull() ) {
                                    if( e2.tagName() == "bookmark" ) {
                                        add_bookmark(new MTCbookmark(e2));
                                    }
                                }
                                n2 = n2.nextSibling();
                            }
                        }
                        if( e1.tagName() == "palettes" ) {
                            QDomNode n2 = n1.firstChild();
                            while( !n2.isNull() ) {
                                QDomElement e2 = n2.toElement();
                                if( !e2.isNull() ) {
                                    if( e2.tagName() == "palette" ) {
                                        bool ok;
                                        QString description=e2.attribute( "description", "" );
                                        long offset=e2.attribute( "offset", "" ).toULong(&ok,16);
                                        QString filename=e2.attribute( "file", "" );
                                        if (filename!="")
                                        {
                                            addPalette(description,filename);
                                        }else{
                                            addPalette(description,offset);
                                        }
                                    }
                                }
                                n2 = n2.nextSibling();
                            }
                        }
                    }
                    n1 = n1.nextSibling();
                }
            }
        }
    }


    m_current_palette_index=m_palettes.size()-1;
    m_rom->set_palette(getPalette());
    std::vector<Tile*>::iterator it;
    for ( it=m_rom->get_tiles()->begin() ; it < m_rom->get_tiles()->end(); it++ )
    {
        (*it)->update_palette(getPalette());
    }

    if (all_ok)
        return start;
    else return 0;
}
