class toFlow{
public:
	string network_packets[1000][7];
	int lines_network;

	bool mark[1000];
	string currentflow[100][7];

	float flow_output[100][3];
	int num_flow = 0;

	ofstream outf;

	void read_network_packets(vector<string> t){

		lines_network = t.size()-1;

		for (int i = 1; i < t.size(); i++)
		{
		string temp = t[i];
		string buf;
		stringstream ss(temp);
		for (int j=0;j<7;j++){
			if(ss >> buf){
				network_packets[i-1][j] = buf;
				}
			}
		}
	}

	vodi generate_flow_output(){
		int count = 0;

		for (int i = 0; i < lines_network-1; i++)
		{
			if(network_packets[i0][0]==currentflow[0][0]&&
				network_packets[i0][1]==currentflow[0][1]&&
				network_packets[i0][2]==currentflow[0][2]&&
				network_packets[i0][3]==currentflow[0][3]&&
				network_packets[i0][4]==currentflow[0][4]&&){
				mark[i0] = true;
			count++

			for (int j0 = 0; j0 < <7; j0++)
			{
				currentflow[count][j0] = network_packets[i0][j0];
				}
			}
		}

		if(count>0){

			flow_output[num_flow][0] = (float)num_flow;

			flow_output[num_flow][1] = ((float)std::stoi(currentflow[count][5])-(float)std::stoi(currentflow[0][5]))/(float)count;

			int sum = 0;
			for (int i0 = 0; i0 <= count; i0++)
				sum += std::stoi(currentflow[i0][6]);
			flow_output[num_flow][2] = (float)sum/(float)(count+1);

			num_flow++;
		}
	}



}



class toCluster{


public:

	float flow[100][2];
	int numOflows;

	bool cluseter[100][100];
	bool cluseter_temp[100][100];
	int medoids[100];
	int medoids_temp[100];
	int k;
	float error;
	float error_temp;
	ofstream outf;

	void inializtion(vect<string> t){
		int k = std::stoi(t[0]);

		string temp = t[1];
		string buf;
		stringstream ss(temp);
		for (int j=0;j<k;j++){
			if(ss >> buf){
				medoids_temp[j]=std::stoi(buf);
			}
		}
		allocate_flows();
		calculate_error();
		transfer_clusterAndMedoids();
		error = error_temp;

	}

	void out_put_result(){
		bool execute = true;
		while(execute){
			execute = can_exchange();
		}
		outf.open("KMedoidsClusters.txt");
		outf<<fixed << setprecision(2) << error <<endl;

		for (int i=0;i<k;i++){
			outf<<medoids[i]<<" ";

			cluster[i][medoids[i]]=true;
		}

		outf<<endl;

		for(int i=0;i<k;i++){
			for(int j=0;j<numOflows;j++){
				if(cluster[i][j]==true){
					outf<<j<<" ";

				}
			}
			outf<<endl;

		}
		outf.close();

	}

	bool can_exchange(){
		for(int h=0;h<numOflows;h++){
			for(int j=0;j<k;j++){
				int ttt = medoids_temp[j];
				if(is_nonMedoids(h)){
					medoids_temp[j] = h;
					allocate_flows();
					calculate_error();
					if(error_temp<error){
						transfer_clusterAndMedoids();
						error = error_temp;
						return true;
					}else{
						medoids_temp[j] = ttt;
					}
				}
			}
		}
		return false
	}

	void allocate_flows(){
		for(int i=0;i<k;i++){
			for (int j = 0; i < numOflows; j++)
			
				cluseter_temp[i][j]=false;
			}
			for (int i = 0; i < numOflows; i++)
			{
				if(is_nonMedoids(i)){
					cluster_temp[find_closest_medoid(i)][i]=true;
				}
			}

		}

		void calculate_error(){
			float sum = 0;
			for (int i = 0; i < k; i++)
			{
				for (int i = 0; i < numOflows; j++)
				{
					if(cluster_temp[i][j]==true){
						sum+=distance(medoids_temp[i],j);
					}
				}
			}
			error_temp = sum;
		}
		bool is_nonMedoids(int a){
			for (int i = 0; i < k; i++)
			{
				if(medoids_temp[i]==a){
					return false;
				}
			}
			return true;
		}

		int find_closest_medoid(int a){
			float dis = 10000;
			int iFinal;
			for (int i = 0; i < k; i++){
				if(distance(medoids_temp[i],a)<dis){
					dis = distance(medoids_temp[i],a);
					iFinal = i;
				}
			}
			return iFinal;
		}

		void transfer_clusterAndMedoids(){
			for (int i = 0; i < k; i++)
			{
				medoids[i] = medoids_temp[i];
			}
			for (int i = 0; i < k; i++)
			{
				for (int j = 0; j < numOflows; j++)
				{
					cluster[i][j] = cluster_temp[i][j]
				}
			}
		}

		float distance (int b, int a){
			float r1 = abs(flow[b][0]-flow[a][0]);
			float r2 = abs(flow[b][1]-flow[a][1]);
			return r1+r2;

		}

		int string_to_int(string a){
			int b;
			stringstream convert(a);
			convert >> b;
			return b;
		}


	};

int main(int argc, char* argv[])
{
	toFlow test;
	read_network_packets(t);
	generate_flow_output();
	print_flow_output();


	toCluster object;

	object.numOflows = test.numOflows;

	for (int i = 0; i < object.numOflows; i++)
	{
		object.flow[i][0] = test.flow_output[i][1];
		object.flow[i][1] = test.flow_output[i][2];

	}
	object.inializtion(t_arg2);
	object.output_result();

	return 0;
}

