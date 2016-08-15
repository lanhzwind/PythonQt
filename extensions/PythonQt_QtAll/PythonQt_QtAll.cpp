/*
*
*  Copyright (C) 2010 MeVis Medical Solutions AG All Rights Reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  Further, this software is distributed without any warranty that it is
*  free of the rightful claim of any third person regarding infringement
*  or the like.  Any license provided herein, whether implied or
*  otherwise, applies only to this software file.  Patent licenses, if
*  any, provided herein do not apply to combinations of this program with
*  other software, or any other product whatsoever.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*  Contact information: MeVis Medical Solutions AG, Universitaetsallee 29,
*  28359 Bremen, Germany or:
*
*  http://www.mevis.de
*
*/

#include "PythonQt_QtAll.h"

#ifdef PYTHONQT_WRAP_QTGUI
void PythonQt_init_QtGui(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTSVG
void PythonQt_init_QtSvg(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTSQL
void PythonQt_init_QtSql(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTNETWORK
void PythonQt_init_QtNetwork(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTCORE
void PythonQt_init_QtCore(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTOPENGL
void PythonQt_init_QtOpenGL(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTXML
void PythonQt_init_QtXml(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTXMLPATTERNS
void PythonQt_init_QtXmlPatterns(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTMULTIMEDIA
void PythonQt_init_QtMultimedia(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTQML
void PythonQt_init_QtQml(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTQUICK
void PythonQt_init_QtQuick(PyObject*);
#endif

#ifdef PYTHONQT_WRAP_QTWEBKIT
void PythonQt_init_QtWebKit(PyObject*);
#endif

namespace PythonQt_QtAll
{
  PYTHONQT_EXPORT void init() {

#ifdef PYTHONQT_WRAP_QTCORE
    PythonQt_init_QtCore(0);
#endif

#ifdef PYTHONQT_WRAP_QTNETWORK
    PythonQt_init_QtNetwork(0);
#endif

#ifdef PYTHONQT_WRAP_QTGUI
    PythonQt_init_QtGui(0);
#endif

#ifdef PYTHONQT_WRAP_QTXML
    PythonQt_init_QtXml(0);
#endif

#ifdef PYTHONQT_WRAP_QTSVG
    PythonQt_init_QtSvg(0);
#endif

#ifdef PYTHONQT_WRAP_QTSQL
    PythonQt_init_QtSql(0);
#endif

#ifdef PYTHONQT_WRAP_QTWEBKIT
    PythonQt_init_QtWebKit(0);
#endif

#ifdef PYTHONQT_WRAP_QTOPENGL
    PythonQt_init_QtOpenGL(0);
#endif

#ifdef PYTHONQT_WRAP_QTXMLPATTERNS
    PythonQt_init_QtXmlPatterns(0);
#endif

#ifdef PYTHONQT_WRAP_QTMULTIMEDIA
    PythonQt_init_QtMultimedia(0);
#endif

#ifdef PYTHONQT_WRAP_QTQML
    PythonQt_init_QtQml(0);
#endif

#ifdef PYTHONQT_WRAP_QTQUICK
    PythonQt_init_QtQuick(0);
#endif

  };
};


