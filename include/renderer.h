/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include"notify-backend.h"

struct RendererPrivate;

/*
 * This class is a mock. It only pretends to render notifications.
 */

class Renderer {
private:

    RendererPrivate *p;

public:
    Renderer();
    ~Renderer();

    void changed(); // Will be a slot in a brave Qt world.
    int numChanges() const;
};

#endif /* RENDERER_HPP_ */
