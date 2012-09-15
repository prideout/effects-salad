_templates.resize(4);
params.resize(_templates.size());
memset(&params[0], 0, sizeof(params[0]) * params.size());

params[0].Thickness = 3;
params[0].TopRadius =  8.0f / 13.0f;
params[0].TetSize = 0.1f;
params[0].NumSides = 5;
params[0].Dest = &_templates[0];
threads.push_back(new tthread::thread(_GenerateBuilding, &params[0]));

params[1].Thickness = 3.0f;
params[1].TopRadius =  1.0f;
params[1].TetSize = 0.1f;
params[1].NumSides = 4;
params[1].Dest = &_templates[1];
threads.push_back(new tthread::thread(_GenerateBuilding, &params[1]));

params[2].Thickness = 3.0f;
params[2].TetSize = 0.1f;
params[2].TopRadius =  1.2f;
params[2].NumSides = 3;
params[2].Dest = &_templates[2];
threads.push_back(new tthread::thread(_GenerateBuilding, &params[2]));

params[3].Thickness = 3.0f;
params[3].TetSize = 0.1f;
params[3].TopRadius = 1;
params[3].NumSides = 32;
params[3].Dest = &_templates[3];
threads.push_back(new tthread::thread(_GenerateBuilding, &params[3]));
