/* Copyright 2015 Samsung Electronics Co., LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.gearvrf;

import org.gearvrf.GVRTransform;
import org.joml.Matrix4f;
import org.joml.Quaternionf;
import org.joml.Vector3f;

public class GVRTransformUtility {

    public static Vector3f getPosition(GVRTransform transform, Vector3f dest){
        return dest.set(transform.getPositionX(), transform.getPositionY(), transform.getPositionZ());
    }

    public static void setPosition(GVRTransform transform, Vector3f src){
        transform.setPosition(src.x, src.y, src.z);
    }

    public static Quaternionf getRotation(GVRTransform transform, Quaternionf dest){
        // Pay attention to Quaternionf's components
        return dest.set(transform.getRotationX(), transform.getRotationY(), transform.getRotationZ(), transform.getRotationW());
    }

    public static void setRotation(GVRTransform transform, Quaternionf src){
        // Pay attention to Quaternionf's components
        transform.setRotation(src.w, src.x, src.y, src.z);
    }

    public static Vector3f getScale(GVRTransform transform, Vector3f dest){
        return dest.set(transform.getScaleX(), transform.getScaleY(), transform.getScaleZ());
    }

    public static void setScale(GVRTransform transform, Vector3f src){
        transform.setScale(src.x, src.y, src.z);
    }
	
	public static Vector3f getPosition(GVRTransform transform){
        return getPosition(transform,new Vector3f());
    }

    public static Quaternionf getRotation(GVRTransform transform){
        return getRotation(transform,new Quaternionf());
    }

    public static Vector3f getScale(GVRTransform transform){
        return getScale(transform,new Vector3f());
    }

    public static Vector3f toVector3f(float[] data,int offset,Vector3f dest){
        return dest.set(
            data[offset++],
            data[offset++],
            data[offset++]
        );
    }

    public static Vector3f toVector3f(float[] data,int offset){
        return toVector3f(data,offset,new Vector3f());
    }

    protected static final float kPi=(float)(Math.PI);
    protected static final float kPiOver2=(float)(Math.PI/2);

    /**
     * Get the euler angles in radians in rotation sequence <tt>XYZ</tt> of this quaternion and store them in the
     * provided parameter <code>eulerAngles</code>.
     *
     * <1> http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
     * <2> 3D.Math.Primer.for.graphics.and.game.development
     *
     * @param eulerAngles
     *          will hold the euler angles in radians
     * @return the passed in vector
     */
    @Deprecated
    public static Vector3f getEulerAnglesXYZ(Quaternionf thiz,Vector3f eulerAngles) {
        if(true){

            thiz.x*=-1f;thiz.y*=-1f;//=> Unity3D

            // Extract sin(pitch)

            float sp=-2.0f*(thiz.y*thiz.z-thiz.w*thiz.x);

            // Check for Gimbel lock, giving slight tolerance for numerical imprecision

            if(Math.abs(sp)>0.9999f) {

                // Looking straight up or down

                eulerAngles.x=kPiOver2*sp;

                // Compute heading, slam bank to zero

                eulerAngles.y=(float) Math.atan2(-thiz.x*thiz.z+thiz.w*thiz.y,0.5f-thiz.y*thiz.y-thiz.z*thiz.z);
                eulerAngles.z=0.0f;

            } else {

                // Compute angles.  We don't have to use the "safe" asin
                // function because we already checked for range errors when
                // checking for Gimbel lock

                eulerAngles.x=(float) Math.asin(sp);
                eulerAngles.y=(float)Math.atan2(thiz.x*thiz.z+thiz.w*thiz.y,0.5f-thiz.x*thiz.x-thiz.y*thiz.y);
                eulerAngles.z=(float)Math.atan2(thiz.x*thiz.y+thiz.w*thiz.z,0.5f-thiz.x*thiz.x-thiz.z*thiz.z);
            }

            thiz.x*=-1f;thiz.y*=-1f;//=> OpenGL
            //=> OpenGL
            return eulerAngles;
        } else {
            return thiz.getEulerAnglesXYZ(eulerAngles);
        }
    }

    // <!-- For Reducing GC -->
    protected static Matrix4f sMatrix4fHelper=new Matrix4f();

    /**
     * Transforms position by matrix4f..
     */
    public static Vector3f transformPoint(Matrix4f matrix4f,Vector3f point,Vector3f dest){
        return matrix4f.transformPoint(point,dest);
    }

    /**
     * Transforms position from local space to world space.
     */
    public static Vector3f transformPoint(GVRTransform transform,Vector3f point,Vector3f dest){
        return transformPoint(sMatrix4fHelper.set(transform.getModelMatrix()),point,dest);
    }

    /**
     * Transforms rotation from world space to local space.
     */
    public static Vector3f inverseTransformPoint(GVRTransform transform,Vector3f point,Vector3f dest){
        return transformPoint(sMatrix4fHelper.set(transform.getModelMatrix()).invert(),point,dest);
    }

    // <!-- For Reducing GC -->
    protected static Vector3f new_Vector3f0=new Vector3f();
    protected static Vector3f new_Vector3f1=new Vector3f();
    protected static Vector3f new_Vector3f2=new Vector3f();

    /**
     * Reference: <a href="http://answers.unity3d.com/questions/467614/what-is-the-source-code-of-quaternionlookrotation.html">http://answers.unity3d.com</a>
     */
	protected static Quaternionf lookRotation(Vector3f forward, Vector3f up,Quaternionf dest){

		forward.normalize();
 
		Vector3f vector = forward.normalize(new_Vector3f0);
		Vector3f vector2 = up.cross(vector,new_Vector3f1).normalize();
		Vector3f vector3 = vector.cross(vector2,new_Vector3f2);
		/*var*/float m00 = vector2.x;
		/*var*/float m01 = vector2.y;
		/*var*/float m02 = vector2.z;
		/*var*/float m10 = vector3.x;
		/*var*/float m11 = vector3.y;
		/*var*/float m12 = vector3.z;
		/*var*/float m20 = vector.x;
		/*var*/float m21 = vector.y;
		/*var*/float m22 = vector.z;
 
 
		float num8 = (m00 + m11) + m22;
		//var /*quaternion*/dest = new Quaternion();
		if (num8 > 0f)
		{
			/*var*/float num = (float)Math.sqrt(num8 + 1f);
			/*quaternion*/dest.w = num * 0.5f;
			num = 0.5f / num;
			/*quaternion*/dest.x = (m12 - m21) * num;
			/*quaternion*/dest.y = (m20 - m02) * num;
			/*quaternion*/dest.z = (m01 - m10) * num;
			return /*quaternion*/dest;
		}
		if ((m00 >= m11) && (m00 >= m22))
		{
			/*var*/float num7 = (float)Math.sqrt(((1f + m00) - m11) - m22);
			/*var*/float num4 = 0.5f / num7;
			/*quaternion*/dest.x = 0.5f * num7;
			/*quaternion*/dest.y = (m01 + m10) * num4;
			/*quaternion*/dest.z = (m02 + m20) * num4;
			/*quaternion*/dest.w = (m12 - m21) * num4;
			return /*quaternion*/dest;
		}
		if (m11 > m22)
		{
			/*var*/float num6 = (float)Math.sqrt(((1f + m11) - m00) - m22);
			/*var*/float num3 = 0.5f / num6;
			/*quaternion*/dest.x = (m10+ m01) * num3;
			/*quaternion*/dest.y = 0.5f * num6;
			/*quaternion*/dest.z = (m21 + m12) * num3;
			/*quaternion*/dest.w = (m20 - m02) * num3;
			return /*quaternion*/dest; 
		}
		/*var*/float num5 = (float)Math.sqrt(((1f + m22) - m00) - m11);
		/*var*/float num2 = 0.5f / num5;
		/*quaternion*/dest.x = (m20 + m02) * num2;
		/*quaternion*/dest.y = (m21 + m12) * num2;
		/*quaternion*/dest.z = 0.5f * num5;
		/*quaternion*/dest.w = (m01 - m10) * num2;
		return /*quaternion*/dest;
	}

    // <!-- For Reducing GC -->
    protected static Vector3f sFwdHelper=new Vector3f();
    protected static Vector3f sUpHelper=new Vector3f();

    public static float getAngle(float x,float y){
        if(x==0f){
            if(y>0f){
                return kPiOver2;
            }else if(y<0f){
                return -kPiOver2;
            }
        }else {
            float a=(float) Math.atan(y/x);
            if(a>0f){
                if(x<0f){
                    a+=kPi;
                }
            }else if(a<0){
                if(x<0f){
                    a+=kPi;
                }
            }
            return a;
        }
        return 0f;
    }

    public static float getYaw(Quaternionf thiz){
        thiz.transform(sFwdHelper.set(0f, 0f,1f),sFwdHelper);
        return getAngle(sFwdHelper.z,sFwdHelper.x);
    }

    public static Quaternionf lookRotate(Quaternionf thiz,Vector3f forward, Vector3f up,Quaternionf dest){
        // <!-- TODO : org.joml.Quaternionf.lookRotate(...) applies a rotation that maps the given direction to the positive Z axis,but the default forward is {0,0,-1}
        sFwdHelper.set(forward);sUpHelper.set(up);
        sFwdHelper.z*=-1f;sUpHelper.z*=-1f;// => Unity
        lookRotation(sFwdHelper,sUpHelper,dest);
        dest.x*=-1f;dest.y*=-1f;// => OpenGL
        return dest;
    }

    /**
     * Transforms rotation by matrix4f.
     */
    public static Quaternionf transformRotation(Matrix4f matrix4f,Quaternionf rotation,Quaternionf dest){
        if(true){
            sFwdHelper.set(0f, 0f,-1f);rotation.transform(sFwdHelper,sFwdHelper);
            sUpHelper.set(0f, 1f, 0f);rotation.transform(sUpHelper,sUpHelper);
            matrix4f.transformDirection(sFwdHelper,sFwdHelper);
            matrix4f.transformDirection(sUpHelper,sUpHelper);
            //
            dest.identity();
            return lookRotate(dest,sFwdHelper,sUpHelper,dest);
        }else {
            return dest.identity();
        }
    }

    /**
     * Transforms rotation from local space to world space.
     */
    public static Quaternionf transformRotation(GVRTransform transform,Quaternionf rotation,Quaternionf dest){
        return transformRotation(sMatrix4fHelper.set(transform.getModelMatrix()),rotation,dest);
    }

    /**
     * Transforms rotation from world space to local space.
     */
    public static Quaternionf inverseTransformRotation(GVRTransform transform,Quaternionf rotation,Quaternionf dest){
        return transformRotation(sMatrix4fHelper.set(transform.getModelMatrix()).invert(),rotation,dest);
    }

    // <!-- For Reducing GC -->
    protected static Vector3f sPosHelper=new Vector3f();
    protected static Quaternionf sRotHelper=new Quaternionf();

    /**
     * Set the parent of the GVRSceneObject.
     *
     *@param sceneObject
     *
     * @param parent
     *           The parent GVRSceneObject to use.
     * @param worldPositionStays
     *           If true, the parent-relative position, scale and rotation is modified such that the object keeps the same world space position, rotation and scale as before.
     */
    public static void setParent(GVRSceneObject sceneObject,GVRSceneObject parent, boolean worldPositionStays){
        GVRSceneObject parentPrev=sceneObject.getParent();
        if(worldPositionStays){
            if(parent==null){
                if(parentPrev==null){
                    // Do nothing...
                }else {
                    GVRTransform parent_t=parentPrev.getTransform();
                    GVRTransform t=sceneObject.getTransform();
                    GVRTransformUtility.transformPoint(parent_t,GVRTransformUtility.getPosition(t,sPosHelper),sPosHelper);
                    GVRTransformUtility.transformRotation(parent_t,GVRTransformUtility.getRotation(t,sRotHelper),sRotHelper);
                    parentPrev.removeChildObject(sceneObject);
                    GVRTransformUtility.setPosition(t,sPosHelper);
                    GVRTransformUtility.setRotation(t,sRotHelper);
                }
            }else {
                if(parentPrev==null){
                    // Do nothing...
                }else {
                    setParent(sceneObject,null,true);
                }
                GVRTransform parent_t=parent.getTransform();
                GVRTransform t=sceneObject.getTransform();
                GVRTransformUtility.inverseTransformPoint(parent_t,GVRTransformUtility.getPosition(t,sPosHelper),sPosHelper);
                GVRTransformUtility.inverseTransformRotation(parent_t,GVRTransformUtility.getRotation(t,sRotHelper),sRotHelper);
                GVRTransformUtility.setPosition(t,sPosHelper);
                GVRTransformUtility.setRotation(t,sRotHelper);
                parent.addChildObject(sceneObject);
            }
        }else {
            if(parentPrev!=null){
                parentPrev.removeChildObject(sceneObject);
            }
            if(parent!=null) {
                parent.addChildObject(sceneObject);
            }
        }
    }

}