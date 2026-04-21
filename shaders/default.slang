static float2 positions[3] = {
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5)
};

static float3 colors[3] = {
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
};

struct VertexOutput
{
    float4 position : SV_Position;
    float3 color;
};

[shader("vertex")]
VertexOutput vertMain(uint vertexID: SV_VertexID)
{
    VertexOutput output;

    output.position = float4(positions[vertexID], 0.0, 1.0);
    output.color = colors[vertexID];

    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput inVertex) : SV_Target
{
    return float4(inVertex.color, 1.0);
}
