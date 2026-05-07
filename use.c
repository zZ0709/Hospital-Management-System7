
 
Patient_UI() {
	char patientid[19];
	search_patientlist();
	add_patientlist();//
	modify_patient(patientid);
    print_hospitalization_bill();
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