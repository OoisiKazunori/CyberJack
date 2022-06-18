
matrix LoadVecToMatrix(float3 POS)
{
    matrix output;
    output[0].r = POS.r;
    output[0].g = POS.g;
    output[0].b = POS.b;
    output[0].a = 0.0f;
    
    for (int i = 1; i < 3; ++i)
    {
        output[i].rgba =
            float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    return output;
}

matrix Translate(float3 VECTOR)
{
    matrix result;
    result[0] = float4(1.0f, 0.0f, 0.0f, 0.0f);
    result[1] = float4(0.0f, 1.0f, 0.0f, 0.0f);
    result[2] = float4(0.0f, 0.0f, 1.0f, 0.0f);
    result[3] = float4(VECTOR.x, VECTOR.y, VECTOR.z, 1.0f);
    return result;
}
    
matrix Scale(float3 VECTOR)
{
    matrix result;
    result[0] = float4(VECTOR.x, 0.0f, 0.0f, 0.0f);
    result[1] = float4(0.0f, VECTOR.y, 0.0f, 0.0f);
    result[2] = float4(0.0f, 0.0f, VECTOR.z, 0.0f);
    result[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return result;
}
    
matrix RotateX(float ANGLE)
{
    float lsin = sin(ANGLE);
    float lcos = sin(ANGLE);
        
    matrix result;
    result[0] = float4(0.0f, 0.0f, 0.0f, 0.0f);
    result[1] = float4(0.0f, lcos, lsin, 0.0f);
    result[2] = float4(0.0f, -lsin, lcos, 0.0f);
    result[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return result;
}
    
matrix RotateY(float ANGLE)
{
    float lsin = sin(ANGLE);
    float lcos = sin(ANGLE);
    
    matrix result;
    result[0] = float4(lcos, 0.0f, -lsin, 0.0f);
    result[1] = float4(0.0f, 1.0f, 0.0f, 0.0f);
    result[2] = float4(lsin, 0.0f, lcos, 0.0f);
    result[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return result;
}
    
matrix RotateZ(float ANGLE)
{
    float lsin = sin(ANGLE);
    float lcos = sin(ANGLE);
    
    matrix result;
    result[0] = float4(lcos, lsin, 0.0f, 0.0f);
    result[1] = float4(-lsin, lcos, 0.0f, 0.0f);
    result[2] = float4(0.0f, 0.0f, 1.0f, 0.0f);
    result[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return result;
}