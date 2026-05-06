pdrug druglisthead = (pdrug)malloc(sizeof(tdrug));
pdrug druglistrear;
pdrug_record drugrecordhead = (pdrug_record)malloc(sizeof(tdrug_record));
pdrug_record drugrecordrear;
ppatuent patienthead = (ppatient)malloc(sizeof(tpatient));
ppatuent patientrear;
pregistration registrationhead = (pregistration)malloc(sizeof(tregistration));
pregistration registrationresr;
patientlist todaylist;
Patient_UI() {
	char patientid[19];
	get_alpha_string("Please enter ID number number:\n", patientid, 19);
	search_patientlist(patientid);
	add_patientlist();//
	modify_patient(patientid);
}

Doctor_UI() {
	search_drug_name();
	pdrug p;
	p=search_druglist();
	drugrecordrear=outbound_drug(Date);//today
	modify_doctor(todaylist);//
	search_patient_name();
}

Manager_UI(){
	druglistrear=add_druglist(Date);//today
	delete_druglist();
	modify_druglist();
	inbound_drug();
}