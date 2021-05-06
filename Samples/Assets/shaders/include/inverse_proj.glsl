mat4 invProjCalc(in mat4 ProjectionMatrix)
{
    mat4 invProj = mat4(0);
    
    float a = ProjectionMatrix[0][0];
    float b = ProjectionMatrix[1][1];
    float c = ProjectionMatrix[2][2];
    float d = ProjectionMatrix[3][2];
    float e = ProjectionMatrix[2][3];
    
    invProj[0][0] = 1 / a;
    invProj[1][1] = 1 / b;
    invProj[2][3] = 1 / d;
    invProj[3][2] = 1 / e;
    invProj[3][3] = -c / (d * e);
    
    return invProj;
}
