#include "Ring.h"


void Ring::getPosition(long long time, ld& x, ld& y, ld& z)
{
	if (orbit_target == NULL)
	{
		x = y = z = 0.0;
	}
	else
	{
		orbit_target->getPosition(time, x, y, z);
	}
}

void Ring::draw()
{
	static const float WHITE[4] = { 1, 1, 1, 1 };

    glPushMatrix();
	
	// Set the color
    glColor3ub( color[0], color[1], color[2] );
	
	// Move into orbit position
    glTranslated( position[0], position[1], position[2] );
	
	// Adjust for right ascention
	glRotated( right_ascension, 0.0, 0.0, 1.0);
	
	// Show planet's axis
	glRotated( rotation_axis, 0.0, 1.0, 0.0);
	
	// Rotate according to planet's rotation
	glRotated( rotation_angle, 0.0, 0.0, 1.0 );
	
	// Gives the object surface normals for light
	// Also apply texture if enabled
	switch (draw_mode)
	{
	case 3: // Wireframe
		glDisable(GL_LIGHTING);

	case 2:	// Flat shading
		glShadeModel( GL_FLAT );
		break;

	case 0:	// Textured mode
		glEnable( GL_TEXTURE_2D );
		if (texture_name != 0)
			glBindTexture(GL_TEXTURE_2D, texture_name);

	case 1: // Smooth shading
		glShadeModel( GL_SMOOTH );
		break;
	
	default:
		break;
	}
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, draw_mode ? lightColor : WHITE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, draw_mode ? lightColor : WHITE);

	glDisable(GL_CULL_FACE);
	glBegin(draw_mode == 3 ? GL_LINE_LOOP : GL_TRIANGLE_STRIP);
	for (ld t = 0, s = 2*M_PI/64;
		t < 2*M_PI;
		t += s)
	{
		glTexCoord2d(1, t / 2*M_PI);
		glVertex2d(cosl(t)*orbit_radius, sinl(t)*orbit_radius);
		glTexCoord2d(0, t / 2*M_PI);
		glVertex2d(cosl(t)*radius, sinl(t)*radius);
	}
	glEnd();
	glEnable(GL_CULL_FACE);

	if (draw_mode == 3)
	{
		// Inner wireframe loop
		glBegin(GL_LINE_LOOP);
		for (ld t = 0, s = 2*M_PI/64;
			t < 2*M_PI;
			t += s)
			glVertex2d(cosl(t)*orbit_radius, sinl(t)*orbit_radius);
		glEnd();

		// Outer wireframe loop
		glBegin(GL_LINE_LOOP);
		for (ld t = 0, s = 2*M_PI/64;
			t < 2*M_PI;
			t += s)
			glVertex2d(cosl(t)*radius, sinl(t)*radius);
		glEnd();
	}

    glPopMatrix();
	glEnable(GL_LIGHTING);
	glDisable( GL_TEXTURE_2D );
}
