
#ifndef NORMAL_DISTRIBUTION_H
#define NORMAL_DISTRIBUTION_H

#include <iostream>
using namespace std;

#include <glm/vec3.hpp>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include <Shader.h>
#include <Marble.h>

#define EPSILON 0.0000001f

class NormalDistribution {
  class Ball {
    glm::vec3 m_pos, m_vel;
    float m_rad, m_mass;
    bool m_canmove;
  public:
    Ball (glm::vec3 pos, float rad=0.3f, float mass=1.f):
      m_pos(pos), m_vel((rand()%2)/1000.0f,(rand()%5)/250.f, 0),
      m_rad(rad), m_mass(mass),
      m_canmove(true)
    {
    }
    glm::vec3 pos() {
      return m_pos;
    }
    glm::vec3 vel() {
      return m_vel;
    }
    float rad() {
      return m_rad;
    }
    void fix(){
      m_canmove = false;
      m_vel = glm::vec3(0.f);
    }
    void update(float dt) {
      if (m_canmove)
        m_pos += m_vel*dt;
    }

    float timeUntil (const Ball &o) {
    	// Two lines, starting each m_pos and other.m_pos
    	// Going in direction m_vel and other.m_vel

      /*
    	ind t;
    	this = m_pos + t*m_vel
    		x = m_x + t*m_u
    		y = m_y + t*m_v
    		z = m_z + t*m_w
    	that = other.m_pos + t*other.m_vel
    		x = m_x + t*m_u
    		y = m_y + t*m_v
    		z = m_z + t*m_w
    	diff = this-that
    		dx = (m_x - o.m_x) + t*(m_u - o.m_u)
    		dy = (m_y - o.m_y) + t*(m_v - o.m_v)
    		dz = (m_z - o.m_z) + t*(m_w - o.m_w)
    	d2 = dot(diff, diff)
    	d2 = [(m_x - o.m_x) + t*(m_u - o.m_u)]^2 + [(m_y - o.m_y) + t*(m_v - o.m_v)]^2 + [(m_z - o.m_z) + t*(m_w - o.m_w)]^2
    	d = sqrt(d2)
    	[m_r + o.m_r]^2 = [(m_x - o.m_x) + t*(m_u - o.m_u)]^2 + [(m_y - o.m_y) + t*(m_v - o.m_v)]^2 + [(m_z - o.m_z) + t*(m_w - o.m_w)]^2
    	[m_r*o.m_r + 2*m_r*o.m_r + o.m_r*o.m_r] = [(m_x-o.m_x)^2 + 2*t*(m_u-o.m_u)*(m_x-o.m_x) + t^2*(m_u-o.m_u)^2] +
    						  [(m_y-o.m_y)^2 + 2*t*(m_v-o.m_v)*(m_y-o.m_y) + t^2*(m_v-o.m_v)^2] +
    						  [(m_z-o.m_z)^2 + 2*t*(m_w-o.m_w)*(m_z-o.m_z) + t^2*(m_w-o.m_w)^2] +
    	t^2 *  [(m_u-o.m_u)^2 + (m_v-o.m_v)^2 + (m_w-o.m_w)^2] +
    	t   * 2[(m_u-o.m_u)*(m_x-o.m_x) + (m_v-o.m_v)*(m_y-o.m_y) + (m_w-o.m_w)*(m_z-o.m_z)] +
    	1   *  [(m_x-o.m_x)^2 + (m_y-o.m_y)^2 + (m_z-o.m_z)^2 - (m_r+o.m_r)^2]
    	a, b, c = ...

    	inner = b^2 - 4*a*c
    	if inner < 0: no solution
    		tmin = -1
    		tmax = -1
    	if inner = 0: 1 solution:
    		tmin = -b / 2*a
    		tmax = tmin
    	if inner > 0: 2 solutions:
    		tmin = [-b - sqrt(b^2 - 4*a*c)] / 2*a
    		tmax = [-b + sqrt(b^2 - 4*a*c)] / 2*a
    	objective: d = m_radius + other.m_radius
    	*/

      if (!m_canmove && !o.m_canmove)
        return -1.f;

    	glm::fvec3 dpos = m_pos - o.m_pos,
                dvel = m_vel - o.m_vel;
      float rad = m_rad + o.m_rad;
      // cout << "R" << m_rad << " @ " << glm::to_string(m_pos) << " with vel " << glm::to_string(m_vel) << endl;
      // cout << "R" << o.m_rad << " @ " << glm::to_string(o.m_pos) << " with vel " << glm::to_string(o.m_vel) << endl;

    	float a =     glm::dot(dvel, dvel);
    	float b = 2.0f * glm::dot(dpos, dvel);
    	float c =    glm::dot(dpos, dpos) - rad*rad;

    	float inner = b*b - 4*a*c;
      // printf("Inner: %.5f\n", inner);
    	if (inner < EPSILON)
    		return -1.f;

    	float one_over_2a = 0.5f / a;

    	float sqrt_inner = sqrt(inner);

    	float tmin = (-b - sqrt_inner) * one_over_2a;
    	float tmax = (-b + sqrt_inner) * one_over_2a;

      // d = t^2*a + t*b + c

    	// if (tmin < EPSILON)
    	// 	return tmax;
      if (2*tmin*a + b >= 0)
        return -1.f;
    	return tmin;
    }
    void processInteraction (Ball &o) {
      // Assume this and o are colliding at their boundaries
      glm::vec3 delta = m_pos - o.m_pos;  // a - b, really doesn't matter

      glm::fvec3 dvel = m_vel - o.m_vel;
    	float b = 2.0f * glm::dot(delta, dvel);

      if (b >= 0 || glm::dot(delta, delta) > (m_rad + o.m_rad)*(m_rad + o.m_rad)) {
        // printf("Not colliding: %.f\n", glm::dot(delta, delta));
        return;
      }
      // impulse to this = K * (o.m_pos - m_pos)
      double K = - 2 * (double)glm::dot(delta, m_vel-o.m_vel) / (double)glm::dot(delta, delta) * (double)(m_mass * o.m_mass) / (double)(m_mass + o.m_mass);

      // Transfer impulses
      // if (m_canmove && o.m_canmove) {
        cout << "AColliding: " << sqrt(glm::dot(delta,delta)) << endl;
        cout << "Vel: " << glm::to_string(o.m_vel) << "\tDelta: " << glm::to_string((float)K*delta/o.m_mass) << endl;
        m_vel += (float)K*delta/m_mass;
        o.m_vel -= (float)K*delta/o.m_mass;
        // o.m_vel *= -1;
      // } else if (m_canmove) {
      //   cout << "BColliding: " << sqrt(glm::dot(delta,delta)) << endl;
      //   m_vel += K*delta/m_mass;
      // } else if (o.m_canmove) {
      //   cout << "CColliding: " << sqrt(glm::dot(delta,delta)) << endl;
      //   o.m_vel -= K*delta/o.m_mass;
      // } else
      // cout << "DColliding: " << sqrt(glm::dot(delta,delta)) << endl;
      /* Wut? */;

    }

    void grav() {
      if (m_canmove)
        m_vel.y -= 0.01f;
    }
    void bouncex() {
      m_vel.x *= -1.0f;
    }
    void bouncey() {
      m_vel.y *= -.96f;
    }

    bool down() {
      return m_vel.y < 0;
    }
    bool left() {
      return m_vel.x < 0;
    }
    bool right() {
      return m_vel.x > 0;
    }

    void setVel(glm::vec3 new_vel) {
      m_vel = new_vel;
    }
  };

  std::vector<Ball> m_balls;
  Marble m_renderObject;
  double m_next;

  void updateAll (double dt) {
    for (int i=0; i<m_balls.size(); i++) {
      Ball &b = m_balls[i];
      b.update((float)dt);
    }
  }

  void setup1(int m_count){
    m_balls.reserve(m_count+3);

    // m_balls.emplace_back(glm::vec3(-1000.f, 0.f, 0.1f), 990.f, 1000.f);
    m_balls.emplace_back(glm::vec3(-5.f, 0.f, 0.1f), 5.f, 1000.f);
    m_balls[0].fix();
    // m_balls.emplace_back(glm::vec3(1000.f, 0.f, 0.1f), 990.f, 1000.f);
    m_balls.emplace_back(glm::vec3(5.f, 0.f, 0.1f), 5.f, 1000.f);
    m_balls[1].fix();
    // m_balls.emplace_back(glm::vec3(0.f, -1000.f, 0.1f), 1000.f, 1000.f);
    m_balls.emplace_back(glm::vec3(0.f, -5.f, 0.1f), 5.f, 1000.f);
    m_balls[2].fix();
    cout << "Ground: " << glm::to_string(m_balls[2].pos()) << m_balls[2].rad() << endl;
    float start_x, start_y, start_z = 0.1f;
    for (int i=0; i<m_count; i++) {
      start_x = rand()%18-9;
      start_y = rand()%10+5;
      start_x = 4; start_y = 5.6f;
      m_balls.emplace_back(glm::vec3(start_x, start_y, start_z), 0.1f);
    }
  }
  void setup2(int m_count){
    m_balls.reserve(2);

    m_balls.emplace_back(glm::vec3(0.f,0.f,0.1f), 1.f, 10000.f);
    m_balls[0].fix();
    m_balls.emplace_back(glm::vec3(0.f,1.5f,0.1f), 0.5f, 1.f);
  }
public:
  NormalDistribution(int m_count):
    m_renderObject(glm::vec3(0.0f), 0.3f)
  {
    srand(time(NULL));

    setup1(m_count);
  }

  void tick() {
    // Apply a constant amount of gravity each tick
    // Within each tick, ball trajectories are modeled as straight lines
    cout << "Grav" << endl;
    for (int i=0; i<m_balls.size(); i++)
      m_balls[i].grav();


    double now = 0.0f;
    double tick_duration = .5f;
    while (now < tick_duration) {
      // Process current collisions
      for (int i=0; i<m_balls.size(); i++) {
        Ball &a = m_balls[i];
        for (int j=i+1; j<m_balls.size(); j++) {
          Ball &b = m_balls[j];
          // cout << "old A @ " << glm::to_string(a.pos()) << " : " << glm::to_string(a.vel()) << endl;
          // cout << "old B @ " << glm::to_string(b.pos()) << " : " << glm::to_string(b.vel()) << endl;
          a.processInteraction(b);
          // cout << "new A @ " << glm::to_string(a.pos()) << " : " << glm::to_string(a.vel()) << endl;
          // cout << "new B @ " << glm::to_string(b.pos()) << " : " << glm::to_string(b.vel()) << endl;
        }
      }

      // Check time for next collision
      cout << endl;
      double next_jump = tick_duration-now;
      for (int i=0; i<m_balls.size(); i++) {
        Ball &a = m_balls[i];
        for (int j=i+1; j<m_balls.size(); j++) {
          Ball &b = m_balls[j];
          double time_to_collide = a.timeUntil(b);
          if (time_to_collide >= 0.f) {
            next_jump = time_to_collide < next_jump ?
              time_to_collide : next_jump;
            cout << "Ball " << i << " going to collide with " << j << " in " << time_to_collide << endl;
          }
          cout << "Time between " << i << " and " << j << " is " << time_to_collide << endl;
        }
      }

      //Update all balls up until the next collision
      // next_jump = 0.1f;
      double pre_energy = 0, post_energy = 0;
      for (int i=0; i<m_balls.size(); i++) {
        pre_energy += glm::dot(m_balls[i].vel(), m_balls[i].vel());
      }
      cout << "Next jump: " << next_jump << endl;
      updateAll(next_jump);
      for (int i=0; i<m_balls.size(); i++) {
        post_energy += glm::dot(m_balls[i].vel(), m_balls[i].vel());
      }
      cout << "Energy: " << 0.5*pre_energy << " --> " << 0.5*post_energy << endl << flush;
      now += next_jump;
    }

  }

  void Render(Shader &shader) {
    for (int i=0; i<m_balls.size(); i++) {
      m_renderObject.SetPos(m_balls[i].pos());
      m_renderObject.SetRad(m_balls[i].rad());
      m_renderObject.Render(shader);
    }
  }

};

#endif
