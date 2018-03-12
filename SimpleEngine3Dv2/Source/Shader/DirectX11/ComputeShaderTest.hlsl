struct CS_INPUT
{
  uint3 groupID : SV_GroupID;
  uint3 groupThreadID : SV_GroupThreadID;
  uint groupIndex : SV_GroupIndex;
  uint3 dispatchThreadID : SV_DispatchThreadID;
};

RWStructuredBuffer<float> test;

[numthreads(1,1,1)]
void main(uint3 groupID : SV_GroupID)
{
  test[groupID.x] = 5.0;
}
