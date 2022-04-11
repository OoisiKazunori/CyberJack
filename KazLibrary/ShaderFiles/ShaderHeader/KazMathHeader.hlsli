
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