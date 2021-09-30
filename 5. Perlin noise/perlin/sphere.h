#ifndef SPHERE_H
#define SPHERE_H
//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "hittable.h"


class sphere : public hittable {
public:
    sphere() {}

    sphere(point3 cen, double r, shared_ptr<material> m)
        : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;

private:
    static void get_sphere_uv(const point3& p, double& u, double& v) {

        // We map the x, y, z coordinates to phi/theta(spherical coordinates) then to u ,v

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};


bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;

    // We update u,v coordinates for texture
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}


#endif